using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//using System.Speech.Recognition; これはwindows7(vistaも？)から邪悪な仕様になったので使ってはならぬ
using SpeechLib; //音声認識をCOMで制御する. (Microsoft Speech Library Object)

namespace DictationFilter
{
    class SpeechRecognizer
    {
        //音声認識オブジェクト
        private SpeechLib.SpInProcRecoContext RecognizerRule = null;
        private SpeechLib.SpInProcRecoContext RecognizerDictation = null;
        //言語モデル
        private SpeechLib.ISpeechRecoGrammar RecognizerGrammarRule = null;
        private SpeechLib.ISpeechRecoGrammar RecognizerGrammarDictation = null;

        //言語モデルのルールのトップレベルオブジェクト.
        private SpeechLib.ISpeechGrammarRule RecognizerGrammarRuleGrammarRule = null;
        //Dictation で認識した文字列.
        private string DictationString = "";
        //必ずマッチしていなければいけない文字列
        private string MustMatchString = "";

        //中継するデリゲート
        public event _ISpeechRecoContextEvents_StartStreamEventHandler StartStream;           //ストリームが開始された時
        public event _ISpeechRecoContextEvents_HypothesisEventHandler Hypothesis;             //認識途中でなんか拾った時
        public event _ISpeechRecoContextEvents_RecognitionEventHandler Recognition;           //認識完了時
        public event _ISpeechRecoContextEvents_FalseRecognitionEventHandler FalseRecognition; //認識失敗時
        public event _ISpeechRecoContextEvents_EndStreamEventHandler EndStream;               //ストリーム終了時

        public event _ISpeechRecoContextEvents_RecognitionEventHandler DictationRecognition;  //Dictationの認識完了時
        public SpeechRecognizer()
        {
            //ルール認識 音声認識オブジェクトの生成
            this.RecognizerRule = new SpeechLib.SpInProcRecoContext();
            this.RecognizerDictation = new SpeechLib.SpInProcRecoContext();
            //マイクから拾ってね。
            this.RecognizerRule.Recognizer.AudioInput = this.CreateMicrofon();
            this.RecognizerDictation.Recognizer.AudioInput = this.CreateMicrofon();

            //イベント設定(中継)
            this.RecognizerRule.Hypothesis +=
                delegate(int streamNumber, object streamPosition, SpeechLib.ISpeechRecoResult result)
                { this.Hypothesis(streamNumber, streamPosition, result); };
            this.RecognizerRule.Recognition +=
                delegate(int streamNumber, object streamPosition, SpeechLib.SpeechRecognitionType srt, SpeechLib.ISpeechRecoResult isrr)
                {
                    //ここでDictationでマッチした語を見て、 必ず入っていなければいけない文字列がなければ握りつぶす.
                    if (    this.MustMatchString.Length >= 1 
                         && this.DictationString.IndexOf(this.MustMatchString) <= -1
                       )
                    {//握りつぶす.
                        this.FalseRecognition(streamNumber, streamPosition, isrr); 
                        return;
                    }
                    this.Recognition(streamNumber, streamPosition, srt, isrr); 
                };
            this.RecognizerRule.StartStream +=
                delegate(int streamNumber, object streamPosition)
                {
                    this.DictationString = ""; //開始時に前回マッチした文字列を消す.
                    this.StartStream(streamNumber, streamPosition); 
                };
            this.RecognizerRule.FalseRecognition +=
                delegate(int streamNumber, object streamPosition, SpeechLib.ISpeechRecoResult isrr)
                { this.FalseRecognition(streamNumber, streamPosition, isrr); };
            this.RecognizerRule.EndStream +=
                delegate(int streamNumber, object streamPosition, bool streamReleased)
                { this.EndStream(streamNumber, streamPosition, streamReleased); };


            //Dictationでマッチした文字列. RuleよりDictationの方がマッチ順は早いらしい。
            this.RecognizerDictation.Recognition +=
                delegate(int streamNumber, object streamPosition, SpeechLib.SpeechRecognitionType srt, SpeechLib.ISpeechRecoResult isrr)
            {
                //マッチした文字列の記録
                this.DictationString = isrr.PhraseInfo.GetText(0, -1, true);
                //コールバック用のデリゲートを呼ぶ.(これくらいあってもいいかな)
                this.DictationRecognition(streamNumber, streamPosition, srt, isrr);
            };

            //言語モデルの作成
            this.RecognizerGrammarRule = this.RecognizerRule.CreateGrammar(0);
            this.RecognizerGrammarDictation = this.RecognizerDictation.CreateGrammar(0);
        }
        //マイクから読み取るため、マイク用のデバイスを指定する.
        // C++ だと SpCreateDefaultObjectFromCategoryId ヘルパーがあるんだけど、C#だとないんだなこれが。
        private SpeechLib.SpObjectToken CreateMicrofon()
        {
            var ObjectTokenCat = new SpeechLib.SpObjectTokenCategory();
            ObjectTokenCat.SetId(@"HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Speech\AudioInput");
            var token = new SpeechLib.SpObjectToken();
            token.SetId(ObjectTokenCat.Default);

            return token;
        }

        //ルールを追加する
        public void SetString(string str)
        {
            //現在のルールをすべて消す.
            this.RecognizerGrammarRule.Reset(0);
            //言語モデルのルールのトップレベルを作成する.
            this.RecognizerGrammarRuleGrammarRule = this.RecognizerGrammarRule.Rules.Add("TopLevelRule",
                SpeechRuleAttributes.SRATopLevel | SpeechRuleAttributes.SRADynamic);

            //文字列の追加.
            this.RecognizerGrammarRuleGrammarRule.InitialState.AddWordTransition(null,this.MustMatchString + str);

            //ルールを反映させる。
            this.RecognizerGrammarRule.Rules.Commit();
            //音声認識開始。(トップレベルのオブジェクトの名前で SpeechRuleState.SGDSActive を指定する.)
            this.RecognizerGrammarRule.CmdSetRuleState("TopLevelRule", SpeechRuleState.SGDSActive);

            //Dictationベースの音声認識もスタート.
            this.RecognizerGrammarDictation.DictationSetState(SpeechRuleState.SGDSActive);
        }

        //絶対入っていないといけない文字列
        public void SetMustMatchString(string str)
        {
            this.MustMatchString = str;
        }
    }
}
