= リモートリポジトリの操作

== リモートリポジトリとは

リモートリポジトリとは、ネットワークの先にあるリポジトリのことです。
リモートリポジトリはコードの保管庫として使用します。
複数人で開発する場合は、まず手元のPCで開発してローカルリポジトリにコミットします。
その後、リモートリポジトリにアップロードしてコードを共有します。

== リモートリポジトリを作成してみる

リモートリポジトリを置くサービスとしてGitHubを使用します。
GitHubアカウントを作成していない場合は、GitHubのホームページ@<fn>{github}からアカウントを作成し、
SSH鍵を登録してください@<fn>{register-ssh-key-to-github}。

//footnote[github][@<href>{https://github.com}]
//footnote[register-ssh-key-to-github][こちらの記事が参考になると思います。@<href>{https://qiita.com/shizuma/items/2b2f873a0034839e47ce}]

GitHubにログインしたら、左上の+アイコンのNew Repositoryをクリックします。

//image[create-repository][リポジトリを作成する]{
//}

Repository nameに「git-practice」と入力し、ページ下部のCreate repositoryボタンをクリックします。
リポジトリを作成すると、以下のようなページが表示されます。

//image[empty-remote-repository][空のリモートリポジトリ]{
//}

== リモートリポジトリへのアップロード

それでは、ローカルリポジトリの内容をリモートリポジトリにアップロードしてみましょう。

新しくローカルリポジトリを作成して、適当にコミットします。

//cmd{
$ mkdir git-practice && cd git-practice
$ git init
$ echo '# git-practice' > README.md
$ git add . && git commit -m 'first commit'
//}

アップロードする前に、リモートリポジトリを登録する必要があります。
リモートリポジトリを登録するには、@<code>{git remote add}コマンドを使用します。

以下のコマンドで、originという名前でリモートリポジトリを登録します。

//cmd{
$ git remote add origin git@github.com:<あなたのGitHubユーザー名>/git-practice.git
//}

リモートリポジトリにアップロードするには、@<code>{git push}コマンドを使用します。
pushの第一引数にはリポジトリ、第二引数にはブランチを指定します。

//cmd{
$ git push origin main # リモートリポジトリoriginにmainブランチをアップロードする
//}

pushが完了すると、GitHubのリポジトリのページにアップロードしたファイルが表示されます。

//image[after-push][プッシュ後のGitHubのリポジトリページ]{
//}

== リモートリポジトリからのダウンロード

複数人で開発すると、リモートリポジトリとローカルリポジトリの内容に差が生まれます。
その場合、リモートの内容をダウンロードしてローカルに反映する必要があります。

この状態を再現するために、別のローカルリポジトリでコミットを作成してpushします。

まずは別のローカルリポジトリを作成します。
すでにリモートリポジトリが存在するため、
@<code>{git clone}コマンドでリモートリポジトリをコピーしてローカルリポジトリを作成します。
@<code>{git clone}コマンドで指定するURLは、GitHubのリポジトリページのCodeボタンをクリックすると見れます。

//cmd{
$ cd ../
$ git clone git@github.com:<ユーザー名>/git-practice.git git-practice2 # git-practice2/にコピー
//}

VSCodeのワークスペースの機能を使うと、複数のディレクトリを同時に開くことができます。

//cmd{
$ code -a git-practice2
//}

もう一方のリポジトリでコミットを作成してpushします。

//cmd{
$ echo '\n- apple' >> README.md
$ git add . && git commit -m 'appleを追加'
$ git push origin main
//}

現在は以下のような状態になっています。

TODO: 図を入れる

それではリモートリポジトリの状態を、1つ目のローカルリポジトリにダウンロードしてみます。
ダウンロードするためには、@<code>{git fetch}コマンドを使います。
fetchコマンドは、pushコマンドと同様にリポジトリ名とブランチ名を指定します。

ブランチ名を省略した場合はすべてのブランチを取得します。
また、リポジトリを省略した場合はoriginが使用されます@<fn>{when-upstream-branch-is-set}。

//footnote[when-upstream-branch-is-set][現在のブランチに上流ブランチが設定されている場合は、そちらが使用されます。]

//cmd{
$ git fetch origin main
//}

これで、リモートリポジトリのコミットをローカルリポジトリに取り込めました。

//image[local1-after-fetch][fetch実行後の1つ目のローカルリポジトリ]{
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
