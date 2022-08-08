= リモートリポジトリの操作

== リモートリポジトリとは

リモートリポジトリとは、ネットワークの先にあるリポジトリのことです。
リモートリポジトリはコードの保管庫として使用します。
複数人で開発する場合は、まず手元のPCで開発してローカルリポジトリにコミットします。
その後、リモートリポジトリにアップロードしてコードを共有します。

== リモートリポジトリを作成してみる

リモートリポジトリを置くサービスとしてGitHubを使用します。
GitHubアカウントを作成していない場合は作成してください。

GitHubにログインしたら、左上の+アイコンのNew Repositoryをクリックします。

//image[create-repository][リポジトリを作成する]{
//}

Repository nameに「git-practice」と入力し、ページ下部のCreate repositoryボタンをクリックします。
リポジトリを作成すると、以下のようなページが表示されます。

//image[empty-remote-repository][空のリモートリポジトリ]{
//}

== リモートリポジトリへのアップロード

それでは、ローカルリポジトリの内容をリモートリポジトリにアップロードしてみましょう。

アップロードする前に、リモートリポジトリを登録する必要があります。
リモートリポジトリを登録するには、@<code>{git remote add}コマンドを使用します。

以下のコマンドで、originという名前でリモートリポジトリを登録します。

//cmd{
$ git remote add origin git@github.com:<あなたのGitHubユーザー名>/git-practice.git
//}

リモートリポジトリにアップロードするには、@<code>{git push}コマンドを使用します。
pushの第一引数にはリポジトリ、第二引数にはブランチを指定します。

//cmd{
$ git push origin main # リモートリポジトリにmainブランチをアップロードする
//}

pushが完了すると、GitHubのリポジトリのページにアップロードしたファイルが表示されます。

TODO: 画像を入れる

== リモートリポジトリからのダウンロード

複数人で開発すると、リモートリポジトリとローカルリポジトリの内容に差が生まれます。
その場合、リモートの内容をダウンロードしてローカルに反映する必要があります。

この状態を再現するために、別のローカルリポジトリでコミットを作成してpushします。

まずは別のローカルリポジトリを作成します。
すでにリモートリポジトリが存在するため、@<code>{git clone}コマンドでリモートリポジトリをローカルにコピーします。

//cmd{
$ cd ../
$ git clone git@github.com:<ユーザー名>/git-practice.git git-practice2 # git-practice2ディレクトリにコピー
//}

もう一方のリポジトリでコミットを作成してpushします。
VSCodeのワークスペースの機能を使うと、複数のディレクトリを開くことができます。

//cmd{
$ code -a ../git-practice2
//}

//cmd{
$ echo 'orange' >> test.txt
$ git add . && git commit -m 'orangeを追加'
$ git push origin main
//}

現在は以下のような状態になりました。

TODO: 図を入れる(前の章や演習の状態を引き継がないほうがよさそう）

//cmd{
# git-practice2、リモートリポジトリ
* 20a4b1b (HEAD -> main, origin/main, origin/HEAD) orangeを追加
* fd0ea24 message4
* 1adc8e4 message2
* c74468a message1

# git-practice1
* fd0ea24 (HEAD -> main) message4
* 1adc8e4 message2
* c74468a message1
//}

 * ローカルリポジトリ1 = A + B
 * ローカルリポジトリ2 = A + B + C
 * リモートリポジトリ = A + B + C

それではリモートリポジトリの状態を、1つ目のローカルリポジトリにダウンロードしてみます。
ダウンロードするためには、@<code>{git fetch}コマンドを使います。

//cmd{
$ git fetch origin main
//}

fetchはpushとちょうど逆の操作です。これで、リモートリポジトリのコミットをローカルリポジトリに取り込めました。

//cmd{
$ git log --all --oneline --graph
* 20a4b1b (origin/main, origin/HEAD) orangeを追加
* fd0ea24 (HEAD -> main) message4
* 1adc8e4 message2
* c74468a message1
//}

== リモート追跡ブランチ

fetchを実行すると、origin/mainというブランチが表れました。
このような、<リモートリポジトリ名>/<ブランチ名>というブランチのことをリモート追跡ブランチといいます。
リモート追跡ブランチは、リモートリポジトリのブランチの位置を表す、ローカルリポジトリにあるブランチです。

TODO: 図を入れる

fetchを実行しただけでは、取得したコミットはまだローカルのmainブランチには反映されていません。
ローカルに反映するためには、リモート追跡ブランチをローカルブランチにマージします。

//cmd{
$ git checkout main
$ git merge origin/main
//}

早送りマージが行われ、ブランチを最新の状態にできました。

== pull

@<code>{git pull}コマンドは、fetchとmergeを同時に行います。

//cmd{
$ git pull origin main # コマンド合ってる?
# 以下のコマンドの略(TODO: 調べて検証する)
$ git fetch origin main
$ git merge origin/main
//}

pullは指定したブランチを最新の状態にするコマンドです。
pullは2つのコマンドの処理が同時に行われるため、挙動がやや複雑なコマンドです。
そのため、はじめはpullを使わずfetchとmergeを使ってみるのがオススメです。

== 上流ブランチ

TODO:
