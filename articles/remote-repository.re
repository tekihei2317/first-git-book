= リモートリポジトリの操作

== リモートリポジトリとは

リモートリポジトリとは、ネットワークの先にあるリポジトリのことです。
リモートリポジトリはコードの保管庫として使用します。
複数人で開発する場合は、まず手元のPCで開発してローカルリポジトリにコミットします。
その後、ローカルリポジトリの内容をリモートリポジトリにアップロードして、ファイルを共有します。

== リモートリポジトリを作成してみる

リモートリポジトリを置くサービスとしてGitHubを使用します。
GitHubアカウントを作成していない場合は、GitHubのホームページ@<fn>{github}からアカウントを作成し、
SSH鍵を登録してください@<fn>{register-ssh-key-to-github}。

//footnote[github][@<href>{https://github.com}]
//footnote[register-ssh-key-to-github][SSH鍵の登録方法はこちらの記事を参照してください。@<href>{https://qiita.com/shizuma/items/2b2f873a0034839e47ce}]

GitHubにログインしたら、左上の+アイコンのNew Repositoryをクリックします。

//image[create-repository][リポジトリを作成する]{
//}

Repository nameに「git-practice」と入力し、ページ下部のCreate repositoryボタンをクリックします。
リポジトリを作成すると、以下のように空のリポジトリページが表示されます。

//image[empty-remote-repository][空のリモートリポジトリ]{
//}

== リモートリポジトリへのアップロード

それでは、ローカルリポジトリの内容をリモートリポジトリにアップロードしてみましょう。

新しくローカルリポジトリを作成して、コミットを作成します。2章で作成したリポジトリは削除しても問題ありません。

//cmd{
$ mkdir git-practice
$ cd git-practice
$ git init
$ echo '# git-practice' > README.md
$ git add . && git commit -m 'first commit'
//}

アップロードする前に、リモートリポジトリをローカルリポジトリに登録する必要があります。
リモートリポジトリを登録するには、@<code>{git remote add}コマンドを使用します。

以下のコマンドで、originという名前でリモートリポジトリを登録します。
このコマンドは、空のリポジトリページの手順に書かれています。

//cmd{
$ git remote add origin git@github.com:<あなたのGitHubユーザー名>/git-practice.git
//}

リモートリポジトリにアップロードするには、@<code>{git push}コマンドを使用します。
pushの第一引数にはリポジトリ、第二引数にはブランチを指定します。

//cmd{
$ git push origin main # リモートリポジトリoriginにmainブランチをアップロードする
//}

プッシュが完了すると、GitHubのリポジトリのページにアップロードしたファイルが表示されます。

//image[after-push][プッシュ後のGitHubのリポジトリページ]{
//}

== リモートリポジトリからのダウンロード

複数人で開発すると、リモートリポジトリとローカルリポジトリの内容に差が生まれます。
その場合、リモートの内容をダウンロードしてローカルに反映する必要があります。

この状態を再現するために、別のローカルリポジトリでコミットを作成してプッシュします。

まずは別のローカルリポジトリを作成します。
すでにリモートリポジトリが存在するため、
@<code>{git clone}コマンドでリモートリポジトリをコピーして作成します。
@<code>{git clone}コマンドで指定するURLは、GitHubのリポジトリページのCodeボタンをクリックすると確認できます。

//cmd{
$ cd ../
$ git clone git@github.com:<ユーザー名>/git-practice.git git-practice2 # git-practice2/にコピー
//}

次に、VSCodeのワークスペースに2つ目のローカルリポジトリのディレクトリを追加します。

//cmd{
$ code -a git-practice2
//}

もう一方のリポジトリでコミットを作成してプッシュします。

//cmd{
$ cd git-practice2
$ echo '\n- apple' >> README.md
$ git add . && git commit -m 'appleを追加'
$ git push origin main
//}

現在は以下のように、2つ目のローカルリポジトリとリモートリポジトリが最新の状態で、
1つ目のローカルリポジトリは1コミット遅れています。

//image[after-push-in-another-repository][ローカルとリモートのリポジトリの状態]{
//}

それではリモートリポジトリの状態を、1つ目のローカルリポジトリにダウンロードしてみます。
ダウンロードするためには、@<code>{git fetch}コマンドを使います。
fetchコマンドは、pushコマンドと同様にリポジトリ名とブランチ名を指定します。

ブランチ名を省略した場合はすべてのブランチを取得します。
また、リポジトリを省略した場合はoriginが使用されます@<fn>{when-upstream-branch-is-set}。

//footnote[when-upstream-branch-is-set][現在のブランチに上流ブランチが設定されている場合は、そちらが使用されます。]

//cmd{
$ cd ../git-practice
$ git fetch origin main
//}

これで、リモートリポジトリのコミットをローカルリポジトリに取り込めました。

//image[local1-after-fetch][fetch実行後の1つ目のローカルリポジトリ]{
//}

== リモート追跡ブランチ

fetchを実行すると、origin/mainというブランチが現れました。
このような、<リモートリポジトリ名>/<ブランチ名>というブランチのことをリモート追跡ブランチといいます。
リモート追跡ブランチは、リモートリポジトリのブランチの位置を表すローカルリポジトリにあるブランチです。
origin/mainは、リモートリポジトリoriginのmainブランチの位置を表します。

//image[remote-tracking-branch][リモート追跡ブランチとは]{
//}

fetchを実行しただけでは、取得したコミットはまだローカルのmainブランチには反映されていません。
ローカルに反映するためには、リモート追跡ブランチをローカルブランチにマージします。

//cmd{
$ git merge origin/main
Updating a3f8c65..c14e53a
Fast-forward
 README.md | 2 ++
 1 file changed, 2 insertions(+)
//}

早送りマージが行われ、ブランチを最新の状態にできました。@<fn>{git-graph-remote-tracking-branch}

//image[merge-remote-tracking-branch][リモート追跡ブランチのマージ後]{
//}

//footnote[git-graph-remote-tracking-branch][Git Graphでは、ローカルブランチとリモート追跡ブランチが同じ位置にある場合、ブランチの後ろにoriginと表示されます。]

== プル

@<code>{git pull}は、リモートリポジトリの変更を現在のブランチに取り込むコマンドです。
@<code>{git pull}は、指定したパラメータで@<code>{git fetch}を実行したあと、@<code>{git merge FETCH_HEAD}を実行します。

//list[git-merge][プルはフェッチとマージを順番に行うコマンド]{
git pull [<repository> [<branch>]]
= git fetch [<repository> [<branch>]] && git merge FETCH_HEAD
//}

@<code>{FETCH_HEAD}は、最後にフェッチしたリモート追跡ブランチの位置を表すポインタです。
つまり@<code>{git pull origin main}は、@<code>{git fetch origin main && git merge origin/main}と同じです。

pullは2つのコマンドの処理が同時に行われるため、挙動がやや複雑なコマンドです。
そのため、はじめはfetchとmergeを使ってリモートリポジトリに関する操作に慣れることをおすすめします。

== 上流ブランチ

=== 上流ブランチとは

上流ブランチとは、ローカルブランチに対応するリモートブランチのことです。
上流ブランチを設定することで、@<code>{git pull}と@<code>{git push}を引数なしで実行できます。

//image[upstream-branch][上流ブランチの設定]{
//}

上流ブランチとは具体的には、gitの設定の@<code>{branch.<name>.remote}と@<code>{branch.<name>.merge}のペアのことです。
この設定は@<code>{.git/config}ファイルで確認できます。

//list[git-upstream-branch][.git/configの上流ブランチの設定]{
# mainブランチの上流ブランチは、リモートリポジトリoriginのmainブランチ
[branch "main"]
	remote = origin
	merge = refs/heads/main
//}

上流ブランチを設定して引数なしで実行すると、@<code>{git pull}は上流ブランチをフェッチしてカレントブランチにマージします。
また、@<code>{git push}はカレントブランチを上流ブランチにプッシュします@<fn>{when-can-push-params-omitted}。

//list[git-push-pull-without-params][引数なしでのpullとpushの実行]{
git pull = git pull [branch.<current>.remote] [branch.<current>.merge]
git push = git push [branch.<current>.remote] <current>
//}

//footnote[when-can-push-params-omitted][pushの引数が省略できるのは、@<code>{push.default}がupstreamまたはsimpleのときです。未設定の場合は暗黙的にsimpleが使用されるため、省略可能です。]

=== 上流ブランチの設定方法

設定方法はいくつかありますが、主に使用するのは以下の2つです。

 * ローカルブランチがある場合 → @<code>{git push}に@<code>{-u}（@<code>{--set-upstream}）オプションをつける
 * ローカルブランチがない場合 → リモート追跡ブランチがある状態で、ローカルブランチをチェックアウトする

ローカルブランチがある場合は、プッシュに@<code>{-u}オプションをつけることで上流ブランチを設定できます。
ローカルブランチがない場合は、リモート追跡ブランチ（例えばorigin/feature）がある状態でローカルブランチ（feature）をチェックアウトすることで、
ブランチの作成と上流ブランチの設定を同時に行えます。
