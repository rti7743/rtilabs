@echo off
echo sshキー設定プログラム by rti

:step0
rem "OpenSSH for windowsがインストールされているかチェック"
ssh -v 2> NUL
if not %ERRORLEVEL% == 1 goto ssh_not_found

:step1
rem ".sshディレクトリがあるかどうかチェック。ないなら作成する"
if NOT EXIST "%USERPROFILE%\.ssh" goto make_ssh_directory

:step2
rem "rsaキーが有るかどうかチェック。ないなら作成する"
if NOT EXIST "%USERPROFILE%\.ssh\id_rsa.pub" goto make_ssh_keygen

:step3
rem "鍵を転送するサーバ名とユーザ名を聞く"
echo.
echo 1.sshキーを登録するサーバ名/IPアドレスを入力してください。
set /p SSH_SERVERNAME="サーバ名>"
echo 2.ユーザ名を入力してください。 何も入れないと現在のユーザ名 %USERNAME% になります。
set /p SSH_USERNAME="ユーザ名>"
if "%SSH_USERNAME%" == "" set SSH_USERNAME="%USERNAME%"

:step4
rem "既に鍵を転送していないか確認する"
echo.
echo なにかキーを押すと、%SSH_USERNAME%@%SSH_SERVERNAME% サーバにsshでログインしキーを登録します。
pause
ssh  -o PreferredAuthentications=publickey %SSH_USERNAME%@%SSH_SERVERNAME% "exit 72"
if %ERRORLEVEL% == 72 goto ssh_already_keyauth

:step5
rem "鍵を転送する"
echo.
type "%USERPROFILE%\.ssh\id_rsa.pub" | ssh %SSH_USERNAME%@%SSH_SERVERNAME% "umask 077; test -d .ssh || mkdir .ssh ; cat >> .ssh/authorized_keys ; exit 72"
if not %ERRORLEVEL% == 72 goto ssh_keycopy_error

:step6
rem "転送でできたか確認する"
echo.
echo 試しにキー認証でログインしてみます。
ssh  -vvv -o PreferredAuthentications=publickey %SSH_USERNAME%@%SSH_SERVERNAME% "exit 72" 2> "%TEMP%\\ssh-copy-id-rti-temp.txt"
if not %ERRORLEVEL% == 72 goto ssh_loginerror
del "%TEMP%\\ssh-copy-id-rti-temp.txt"

:step7
rem "おしまい"
echo.
echo OK.キー認証が完了しました。
echo 何かキーを押すと終了します。
pause
exit 1

:ssh_already_keyauth
echo.
echo あれ？もうすでにキー認証できますよ。
echo 何かキーを押すと終了します。
pause
exit 1

:ssh_loginerror
echo.
echo キー認証でログインできませんでした。
echo サーバの設定がおかしいのかな？
echo 以下詳細なエラーフロー
type "%TEMP%\\ssh-copy-id-rti-temp.txt"
del "%TEMP%\\ssh-copy-id-rti-temp.txt"
echo なにかキーを押すと、キーコピーからやり直します
pause
goto step3


:ssh_keycopy_error
echo.
echo sshキーをコピーできませんでした。。。
echo サーバ名やユーザー名は間違っていませんか？
echo なにかキーを押すともう一度やります
pause
goto step3

:make_ssh_directory
echo.
mkdir "%USERPROFILE%\.ssh"
goto step1

:make_ssh_keygen
echo.
echo sshキー(rsa)が見つかりません。
echo 何かキーを押すと、ssh-keygenを起動してsshキーを作成します
pause
cd %USERPROFILE%\.ssh
echo ノンパスでやりたい人はenterキーを3回連打してくださいwww
echo.
ssh-keygen -t rsa
goto step2

:ssh_not_found
echo.
echo ssh not found.
echo OpenSSHをインストールして下さい
pause
exit 1

