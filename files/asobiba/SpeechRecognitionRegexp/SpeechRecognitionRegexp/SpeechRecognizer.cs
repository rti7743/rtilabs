using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//using System.Speech.Recognition; これはwindows7(vistaも？)から邪悪な仕様になったので使ってはならぬ
using SpeechLib; //音声認識をCOMで制御する. (Microsoft Speech Library Object)

namespace SpeechRecognitionRegexp
{
    class SpeechRecognizer 
    {
        //音声認識オブジェクト
        private SpeechLib.SpInProcRecoContext RecognizerRule = null;
        //言語モデル
        private SpeechLib.ISpeechRecoGrammar RecognizerGrammarRule = null;
        //言語モデルのルールのトップレベルオブジェクト.
        private SpeechLib.ISpeechGrammarRule RecognizerGrammarRuleGrammarRule = null;
        //中継するデリゲート
        public event _ISpeechRecoContextEvents_StartStreamEventHandler StartStream;           //ストリームが開始された時
        public event _ISpeechRecoContextEvents_HypothesisEventHandler Hypothesis;             //認識途中でなんか拾った時
        public event _ISpeechRecoContextEvents_RecognitionEventHandler Recognition;           //認識完了時
        public event _ISpeechRecoContextEvents_FalseRecognitionEventHandler FalseRecognition; //認識失敗時
        public event _ISpeechRecoContextEvents_EndStreamEventHandler EndStream;               //ストリーム終了時

        public SpeechRecognizer()
        {
            //ルール認識 音声認識オブジェクトの生成
            this.RecognizerRule = new SpeechLib.SpInProcRecoContext();
            //マイクから拾ってね。
            this.RecognizerRule.Recognizer.AudioInput = this.CreateMicrofon();

            //イベント設定(中継)
            this.RecognizerRule.Hypothesis +=
                delegate(int streamNumber, object streamPosition, SpeechLib.ISpeechRecoResult result)
                { this.Hypothesis(streamNumber, streamPosition, result); };
            this.RecognizerRule.Recognition +=
                delegate(int streamNumber, object streamPosition, SpeechLib.SpeechRecognitionType srt, SpeechLib.ISpeechRecoResult isrr)
                { this.Recognition(streamNumber, streamPosition, srt, isrr); };
            this.RecognizerRule.StartStream +=
                delegate(int streamNumber, object streamPosition)
                { this.StartStream(streamNumber, streamPosition); };
            this.RecognizerRule.FalseRecognition +=
                delegate(int streamNumber, object streamPosition, SpeechLib.ISpeechRecoResult isrr)
                { this.FalseRecognition(streamNumber, streamPosition, isrr); };
            this.RecognizerRule.EndStream +=
                delegate(int streamNumber, object streamPosition, bool streamReleased)
                { this.EndStream(streamNumber, streamPosition, streamReleased); };

            //言語モデルの作成
            this.RecognizerGrammarRule = this.RecognizerRule.CreateGrammar(0);
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

        //音声認識でルールを追加する
        public void SetRegexp(string str)
        {
            //現在のルールをすべて消す.
            this.RecognizerGrammarRule.Reset(0);
            //言語モデルのルールのトップレベルを作成する.
            this.RecognizerGrammarRuleGrammarRule = this.RecognizerGrammarRule.Rules.Add("TopLevelRule",
                SpeechRuleAttributes.SRATopLevel | SpeechRuleAttributes.SRADynamic);
//            this.RecognizerGrammarRuleGrammarRule.Clear();
            //正規表現でルールを追加する.
            this.AddRegexp(str , this.RecognizerGrammarRule , this.RecognizerGrammarRuleGrammarRule);
            //ルールを反映させる。
            this.RecognizerGrammarRule.Rules.Commit();
            //音声認識開始。(トップレベルのオブジェクトの名前で SpeechRuleState.SGDSActive を指定する.)
            this.RecognizerGrammarRule.CmdSetRuleState("TopLevelRule", SpeechRuleState.SGDSActive);
        }

        //音声認識でルールを追加するの実装部(ムダに長い)
        private void AddRegexp
            (string str, ISpeechRecoGrammar grammer, ISpeechGrammarRule topRule)
        {
            string matchString;

            int i;
            if ((topRule.Attributes & SpeechRuleAttributes.SRATopLevel) == SpeechRuleAttributes.SRATopLevel)
            {
                //一番最初だけ正規表現の構文変換をかける.
                // .+ --> (:?.*)
                // (まる|さんかく)? --> (まる|さんかく|)   正しい正規表現としてはエラーだが、このエンジンの場合容認する.
                // なのは? --> なの(は|)
                string optstr = "";
                for(i = 0 ; i < str.Length ;++i)
                {
                    if (i+1 < str.Length && str[i] == '.' && str[i + 1] == '+')
                    { // .+ --> (:?.*)
                        optstr += "(?:.+)";
                        i += 1;
                    }
                    else if (i + 1 < str.Length && str[i + 1] == '?')
                    {
                        if (str[i] == ')')
                        {// (まる|さんかく)? --> (まる|さんかく|)
                             optstr += "|)";
                        }
                        else 
                        {// なのは? --> なの(は|)
                             optstr += "(?:" + str[i] + "|)";
                        }
                        i += 1;
                    }
                    else if (str[i] == '*' || str[i] == '+' || str[i] == '.' || str[i] == '[' || str[i] == ']')
                    {
                        throw new Exception("現在は、メタ文字 " + str[i] + " は利用できません。利用可能なメタ文字 () | .+ ?");
                    }
                    else
                    {
                        optstr += str[i];
                    }
                }
                str = optstr;
            }

            //正規表現をパースしながら回す.
            int splitPos = 0;
            ISpeechGrammarRule currentRule = topRule;
            for (i = 0;  i < str.Length; ++i)
            {
                if (str[i] == '(')
                {
                    //閉じ括弧まで飛ばす. )
                    int nest = 1;
                    int n = i + 1;
                    for( ; n < str.Length; ++n )
                    {
                        if (str[n] == '(')
                        {
                            ++nest ;
                        }
                        else if (str[n] == ')')
                        {
                            --nest ;
                            if (nest <= 0)
                            {
                                break;
                            }
                        }
                    }

                    //ネスとする前の部分
                    matchString = str.Substring(splitPos, i - splitPos);

                    //キャプチャー？
                    if (str[i + 1] == '?' && str[i + 2] == ':')
                    {
                        i += 2;
                    }

                    ISpeechGrammarRule nestRule = grammer.Rules.Add(
                        currentRule.Name + "_" + i, SpeechRuleAttributes.SRADynamic);
                    ISpeechGrammarRuleState nestRuleState = currentRule.AddState();

                    //ネストする前の部分を挿入.
                    currentRule.InitialState.AddWordTransition(nestRuleState, matchString);

                    //かっこの後にも構文が連続する場合、そのツリーを作成する.
                    if (n+1 < str.Length && (str[n + 1] != '|'))
                    {
                        ISpeechGrammarRule afterRule = grammer.Rules.Add(
                            currentRule.Name + "_af" + +i, SpeechRuleAttributes.SRADynamic);
                        ISpeechGrammarRuleState afterRuleState = currentRule.AddState();
                        nestRuleState.AddRuleTransition(afterRuleState, nestRule);
                        afterRuleState.AddRuleTransition(null, afterRule);
                        currentRule = afterRule;
                    }
                    else
                    {
                        //かっこで構文がとまる場合はそこで終端
                        nestRuleState.AddRuleTransition(null, nestRule);
                    }

                    //ネストしているルールを再帰して実行.
                    matchString = str.Substring(i + 1, n - i - 1);
                    this.AddRegexp(matchString, grammer, nestRule);

                    i = n ;
                    splitPos = n + 1;  //+1は最後の ) を飛ばす. iは forの ++i で i == splitPos となる。(わかりにくい)
                }
                else if (str[i] == '|') 
                {
                    matchString = str.Substring(splitPos, i - splitPos);
                    if (matchString.Length >= 1)
                    {
                        currentRule.InitialState.AddWordTransition(null, matchString);
                    }

                    splitPos = i + 1;
                    currentRule = topRule;
                }
                else if (str[i] == '.' && str[i+1] == '+') 
                {
                    currentRule.InitialState.AddSpecialTransition(null, SpeechSpecialTransitionType.SSTTDictation);
                    i += 1;
                    splitPos = i + 1;
                }
            }
            //最後の残り
            matchString = str.Substring(splitPos, i - splitPos);
            if (str.Length >= 1 && str[i - 1] != ')')
            {
                currentRule.InitialState.AddWordTransition(null, matchString);
            }
        }
    }
}
