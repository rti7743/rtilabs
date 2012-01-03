USB制御できる電源タップ PTU2F3 をコマンドラインから制御するソースです。
PTU2F3Command.exe 4 on
PTU2F3Command.exe 4 off
PTU2F3Command.exe 5 on
PTU2F3Command.exe 5 off

とかして使います。

PTU2F3 の制御用DLL pcdll.dll がバグっていて内部で見初期化メモリを参照しているみたい。
だけど、 pcdll.dll のソースはないし、仕様も公開されていないので、強引にエラーを握りつぶしています。
非常にいけていない。。。

動けばいいのか。
