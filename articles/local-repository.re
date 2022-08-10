= ローカルリポジトリの操作

== 基本的な用語の説明

gitではチェックインする（履歴を登録する）前に、チェックインするファイルを選択することができます。
選択したファイルが記録される場所のことを、インデックスまたはステージングエリアといいます。
また、チェックインする場所のことをリポジトリといいます。また、リポジトリに履歴を登録することをコミットするといいます。
インデックスの仕組みによって、記録したいファイルのみを選択してリポジトリに登録することができます。

TODO: 図を入れる

== 手を動かして実践

それでは、チェックインを実際に手を動かして実践してみましょう。
まず、適当なディレクトリを作成してVSCodeで開きます。

//cmd{
$ mkdir git-practice
$ code git-practice
//}

次に、リポジトリを作成してファイルを追加します。
リポジトリを作成するには@<code>{git init}コマンドを使用します。

//cmd{
$ git init
$ echo "My first file" > test.txt
//}

@<code>{git status}コマンドでリポジトリの状態を確認します。

//cmd{
$ git status
On branch main

No commits yet

Untracked files:
  (use "git add <file>..." to include in what will be committed)
        test.txt

nothing added to commit but untracked files present (use "git add" to track)
//}

追加したファイルがUntracked files（未追跡のファイル）になっています。
ステージしてから、再度@<code>{git status}を実行してみましょう。

//cmd{
$ git add .
$ git status
On branch main

No commits yet

Changes to be committed:
  (use "git rm --cached <file>..." to unstage)
        new file:   test.txt

//}

Changes to be commitedに変わりました。

また、@<code>{git ls-files --stage}でインデックスにあるファイルを確認することができます。
test.txtがインデックスに登録されたことが分かります。

//cmd{
$ git ls-files --stage
100644 363d8b784900d74b3159e8e93a651c0db42629ef 0       test.txt
//}

最後に、@<code>{git commit}でリポジトリにチェックインします。
@<code>{-m}でコミットメッセージを指定します。

//cmd{
$ git commit -m 'first commit'
[main (root-commit) 87ca1d1] first commit
 1 file changed, 1 insertion(+)
 create mode 100644 test.txt
//}

これでコミットが完了しました。
VSCodeの左下のGit Graphをクリックすると、先程作成したコミットを確認できます。

//image[first-commit][最初のコミット]{
//}

=== 練習問題2-1

ファイル`test2.txt`を作成して、コミットしてみましょう。
コミットメッセージは'add test2.txt'とします。

== コミットとは

コミットの実態は、gitで管理しているファイルのスナップショットです。
addで差分を登録してからコミットするため、コミットは差分が記録されていると誤解されがちです。
しかし、コミットはコミットした時点でのすべてのファイルの状態を表しています。

TODO: コミットは差分ではなくスナップショットの図を入れる

== コミットグラフとは

コミット同士を、親コミットへの参照でつないたものをコミットグラフといいます。
親コミットとは、直前に作成したコミットのことです。

コミットグラフは、ブランチやマージなどのgitの基本的な概念を理解するために重要です。

コミットグラフは@<code>{git log --graph}コマンドでも確認できます。
@<code>{--oneline}オプションをつけると、コミットメッセージの1行目だけが表示されて見やすくなります。

//cmd{
$ git log --graph --oneline
* 21180e3 (HEAD -> main) add test2.txt
* 87ca1d1 first commit
//}

VSCodeの拡張機能のGit Graphを使うと、コマンドを実行した際にコミットグラフがリアルタイムに変化します。
また、履歴が分岐して複雑になったときは@<code>{git log}コマンドより見やすいです。
そのため、はじめはGit Graphを使って操作の結果を確認することをおすすめします。

== コミットとブランチ

ブランチは、gitで履歴を分岐させるときに使用するものです。
ブランチ（= 枝）という名前から、ブランチが複数のコミットを表していると勘違いしやすいです。
しかし、ブランチの正体は単一のコミットへの参照です。
コミットについているラベルのようなものと考えると分かりやすいかもしれません。

TODO: ブランチはコミットへの参照の図を入れる

それでは、ブランチを作成してコミットしてみましょう。
ブランチの確認と作成は@<code>{git branch}コマンドを使います。ブランチ間の移動は@<code>{git checkout}または@<code>{git switch}コマンドを使います。
また、@<code>{git checkout -b}でブランチの作成と移動を同時に行えます。

//cmd{
$ git branch # ブランチを確認
* main

$ git branch practice1 # practice1ブランチを作成

$ git branch # practice1ブランチが作成されたことを確認
* main
  practice1

$ git checkout practice1 # practice1ブランチに移動

$ git branch
  main
* practice1
//}

@<code>{*}のマークが移動したことから、practice1ブランチに移動できたことが分かります。

次に、適当にファイルを変更してコミットしてみます。

//cmd{
$ echo 'Hello' >> test.txt
$ git add .
$ git commit -m 'test.txtにHelloを追加'
//}

コミットグラフを確認してみます。

TODO: 図を入れる

新たにコミットが作成され、practice1ブランチが移動しました。以前いたmainブランチの位置は変わっていません。
このように、あるブランチでコミットすると、そのブランチが新規コミット（= 次のコミット）を指し示すように移動します。

=== 練習問題2-2

以下のコミットグラフを作成してみてください。

//list[practice2-2][練習問題2-2]{
* fd0ea24 (HEAD -> main) commit4
| * a1b7cf9 (practice) commit3
|/
* 1adc8e4 commit2
* c74468a first commit
//}

//image[practice-2-2][Git Graph]{
//}

== マージと2種類のマージ

あるブランチで作成したコミットを別のブランチに取り込むときは、@<code>{git merge}コマンドを使います。

gitのマージには、早送りマージ（Fast-forward merge）と早送りでないマージ（Non Fast-forward merge）の2種類あります。

早送りマージは、マージ元のブランチがマージ先のブランチへと移動するマージです。
マージ先ブランチの親コミットをたどってマージ元ブランチへとたどり着けるときに行えます。
gitは早送りマージが可能なときは早送りマージを行います。

TODO: 図を入れる

早送りでないマージでは、新しくコミットが作成されて2つのブランチが合流します。
新しく作成されるコミットのことをマージコミットといいます。

== マージをやってみる

@<code>{git merge}で1つのブランチを指定すると、そのブランチと現在のブランチがマージされます。

mainブランチに戻ってから、practice1ブランチをマージしてみましょう。

//cmd{
$ git checkout main
$ git merge practice1
Updating 21180e3..09471f0
Fast-forward
 test.txt | 1 +
 1 file changed, 1 insertion(+)
//}

Fast-forwardと書かれていることから、早送りマージが行われたことが分かります。
コミットグラフを確認すると、practice1ブランチがmainブランチの位置に移動しています。

TODO: 図を入れる

次は早送りでないマージをしてみましょう。
@<code>{--no-ff}オプションをつけることで、早送りマージが可能なときも早送りでないマージを強制できます。

//cmd{
$ git checkout -b practice2
$ echo 'Hello2' > test.txt
$ git add . && git commit -m 'test.txtにHello2を追加'
$ git checkout main
$ git merge --no-ff practice2
Merge made by the 'recursive' strategy.
 test.txt | 1 +
 1 file changed, 1 insertion(+)
//}

TODO: 図を入れる

mergeコマンドを実行するとエディタが開くため、保存して終了しましょう。
コミットグラフを確認すると、マージコミットが作成されてブランチが合流したことが分かります。

=== 練習問題2-3

以下のコミットグラフを作成してみてください。

//list[practice-2-3][練習問題2-3]{
* 0fca72d (HEAD -> practice2) commit4
*   4990332 (main) commit3
|\
| * 87f630b (practice1) commit2
|/
* e513abf first commit
//}

//image[practice-2-3][Git Graph]{
//}

== リベース

TODO:
