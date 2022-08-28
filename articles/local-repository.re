= ローカルリポジトリの操作

== 基本的な用語の説明

ファイルの履歴が記録される場所や、記録したもののことをリポジトリといいます。
gitでは、履歴をリポジトリに登録する前に、ファイルを選択してインデックス@<fn>{git-index}というファイルの一覧に登録します。
そして、インデックスをもとに履歴を作成します。作成した履歴のことをコミットといいます。

ファイルをインデックスに登録することをステージするといいます。
また、コミットを作成することをコミットするといいます。
ステージはgit addコマンド、コミットはgit commitコマンドで行います。

//footnote[git-index][ステージングエリアともいいます。]

//image[checkin-flow][履歴を登録する流れ]{
//}

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

@<code>{git status}コマンドで現在の状態を確認します。

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

解答: @<hd>{練習問題の解答}

== コミットはスナップショット

addでファイルを選択して登録したり、git statusで差分を確認するため、コミットには差分が記録されていると誤解しがちです。
しかし、コミットは、その時点で管理していたすべてのファイルの状態をもつスナップショットです。

//image[commit-is-snapshot][コミットはスナップショット]{
//}

=== インデックスもスナップショット

インデックスも、すべてのファイルの状態をもつスナップショットです。
コミットはインデックスをもとに作成されます。
インデックスはしばしば変更されるため、インデックスは一時的なスナップショットで、
コミットは永続的なスナップショットであるといえるでしょう。

=== インデックスとコミットの構造

addを実行すると、指定したパスにあるファイルが圧縮されてリポジトリに保存されます。
そして、圧縮されたファイルに対応するハッシュ値や、元のファイルのファイルパスがインデックスに記録されます。

commitを実行すると、インデックスからコミットが作成され、リポジトリに保存されます。
インデックスとコミットは、ファイルのもち方が一次元の配列か木構造かという違いがあります。

//image[index-and-commit][インデックスとコミット]{
//}

== コミットグラフとは

コミット同士を、親コミットへの参照でつないたものをコミットグラフといいます。
親コミットとは、直前に作成したコミットのことです。

コミットグラフは、ブランチやマージなどのgitの基本的な概念を理解するために重要です。

コミットグラフは、VSCodeの拡張機能のGit Graphや、@<code>{git log --graph}コマンドで確認できます。
@<code>{--oneline}オプションをつけると、コミットメッセージの1行目だけが表示されて見やすくなります。

//cmd{
$ git log --graph --oneline
* 21180e3 (HEAD -> main) add test2.txt
* 87ca1d1 first commit
//}

Git Graphは、コマンドを実行した際にコミットグラフがリアルタイムに変化します。
また、履歴が分岐して複雑になったときに、@<code>{git log}コマンドよりコミットグラフの状態を把握しやすいです。
そのため、はじめはGit Graphを見ながら操作することをおすすめします。

== コミットとブランチ

ブランチは、gitで履歴を分岐させるときに使用するものです。
ブランチは、複数人で作業する場合や、作業単位を分かりやすくするために役立ちます。
ブランチ（= 枝）という名前から、ブランチが複数のコミットを表していると勘違いしやすいです。
しかし、ブランチの正体は単一のコミットへの参照です。

//image[branch-is-reference][ブランチは単一のコミットへの参照]{
//}

gitのブランチの実体は単なるファイルです。
例えば、mainブランチを表すファイルは@<code>{.git/refs/heads/main}です。
このファイルを確認すると、コミットを表すハッシュ値だけが書かれています。

//cmd{
$ cat .git/refs/heads/main
dbf07a60c2a7cfc973e8509dc144a6d07455ad14
//}

それでは、ブランチを作成してコミットしてみましょう。
ブランチの確認と作成は@<code>{git branch}コマンドを使います。ブランチ間の移動は@<code>{git checkout}または@<code>{git switch}コマンドを使います。
また、@<code>{git checkout -b}でブランチの作成と移動を同時に行えます。

//cmd{
$ git branch # ブランチを確認
* main

$ git branch practice # practiceブランチを作成

$ git branch # practiceブランチが作成されたことを確認
* main
  practice

$ git checkout practice # practiceブランチに移動

$ git branch
  main
* practice
//}

@<code>{*}のマークが移動したことから、practiceブランチに移動できたことが分かります。

次に、適当にファイルを変更してコミットしてみます。

//cmd{
$ echo 'Hello' >> test.txt
$ git add .
$ git commit -m 'test.txtにHelloを追加'
//}

コミットグラフを確認してみます。

//image[commit-graph-branch][Git Graph]{
//}

新たにコミットが作成され、practicブランチはそのコミットを参照しています。以前いたmainブランチの位置は変わっていません。
このように、新たにコミットを作成すると、現在のブランチが新規コミット（= 次のコミット）を指し示すように移動します。

//image[branch-moving][ブランチは新規コミットを指し示すように移動する]{
//}

=== 練習問題2-2

新たにリポジトリを作成し、以下のコミットグラフを作成してみてください。
commitコマンドに@<code>{--allow-empty}オプションをつけると、変更がない場合もコミットを作成できます。

//list[practice2-2][練習問題2-2]{
* fd0ea24 (HEAD -> main) commit4
| * a1b7cf9 (practice) commit3
|/
* 1adc8e4 commit2
* c74468a first commit
//}

//image[practice-2-2][Git Graph]{
//}

解答: @<hd>{練習問題の解答}

== マージと2種類のマージ

分岐した2つの履歴（ブランチ）を合流させたいときは、@<code>{git merge}コマンドを使います。

gitのマージには、早送りマージ（Fast-forward merge）と早送りでないマージ（Non Fast-forward merge）の2種類あります。

早送りマージは、現在のブランチが取り込むブランチへと移動するマージです。
現在のブランチが、取り込むブランチの祖先のコミットであるときに行われます。
現在のブランチ（過去）が、取り込むブランチ（未来）に移動するため、早送りマージといいます。

//image[fast-forward-merge][早送りマージ]{
//}

早送りでないマージでは、マージする2つのコミットの状態が合わさった、新しいコミットが作成されます。
新しく作成されるコミットのことをマージコミットといいます。

//image[non-fast-forward-merge][早送りでないマージ]{
//}

== マージをやってみる

@<code>{git merge}で1つのブランチを指定すると、そのブランチと現在のブランチがマージされます。

mainブランチに戻ってから、practiceブランチをマージしてみましょう。

//cmd{
$ git checkout main
$ git merge practice
Updating e5291b6..8807efc
Fast-forward
 test.txt | 1 +
 1 file changed, 1 insertion(+)
//}

Fast-forwardと書かれていることから、早送りマージが行われたことが分かります。
コミットグラフを確認すると、mainブランチがpracticeブランチの位置に移動しています。

//image[merge-practice-into-main][practiceブランチをmainブランチに早送りマージ]{
//}

次は早送りでないマージをしてみましょう。まずは新しいブランチを作成してコミットします。

//cmd{
$ git checkout -b practice2
$ echo 'Hello2' >> test.txt
$ git add . && git commit -m 'test.txtにHello2を追加'
//}

mergeコマンドに@<code>{--no-ff}オプションをつけると、早送りマージが可能なときも早送りでないマージを強制できます。

//cmd{
$ git checkout main
$ git merge --no-ff practice2
Merge made by the 'recursive' strategy.
 test.txt | 1 +
 1 file changed, 1 insertion(+)
//}

mergeコマンドを実行するとエディタが開くため、保存して終了しましょう。
コミットグラフを確認すると、マージコミットが作成されたことが分かります。
また、test.txtを確認すると、マージしたpractice2ブランチの変更が取り込まれています。

//image[merge-practice2-into-main][practice2ブランチをmainブランチに早送りでないマージ]{
//}

=== 練習問題2-3

新たにリポジトリを作成し、以下のコミットグラフを作成してみてください。

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

解答: @<hd>{練習問題の解答}

== 練習問題の解答

=== 練習問題2-1の解答

@<code>{git add}でインデックスに追加してから、@<code>{git commit}でコミットします。

//cmd{
$ touch test2.txt
$ git add .
$ git commit -m 'add test2.txt'
//}

問題に戻る: @<hd>{手を動かして実践}

=== 練習問題2-2の解答

commit2でmainブランチとpracticeブランチが分岐しているため、
mainブランチでcommit2まで作成してからpracticeブランチを作成します。

//cmd{
$ git commit --allow-empty -m 'first commit'
$ git commit --allow-empty -m 'commit2'
$ git checkout -b practice
$ git log --graph --oneline
* 2b12ccf (HEAD -> practice, main) commit2
* f1dd836 first commit
//}


ブランチを作成したら、枝分かれするようにそれぞれのブランチでコミットしましょう。

//cmd{
$ git commit --allow-empty -m 'commit3'
$ git checkout main
$ git commit --allow-empty -m 'commit4'
//}

これでコミットグラフは完成です。

問題に戻る: @<hd>{コミットとブランチ}

//cmd{
$ git log --graph --oneline --all
* df297c3 (HEAD -> main) commit4
| * 8c115e9 (practice) commit3
|/
* 2b12ccf commit2
* f1dd836 first commit
//}

=== 練習問題2-3の解答

コミットグラフを見ると、practice1ブランチをmainブランチに早送りでないマージをしていることが分かります。

まず、practice1ブランチを作成してコミットします。

//cmd{
$ git commit --allow-empty -m 'first commit'
$ git checkout -b practice1
$ git commit --allow-empty -m 'commit2'
//}

次に、mainブランチにpractice1ブランチをNon Fast-Forwardマージします。

//cmd{
$ git checkout main
$ git merge --no-ff -m 'commit3' practice1
$ git log --graph --oneline
*   2747816 (HEAD -> main) commit3
|\
| * 86f624d (practice1) commit2
|/
* e8581d0 first commit
//}

最後にpractice2ブランチでコミットして完成です。

//cmd{
$ git checkout -b practice2
$ git commit --allow-empty -m 'commit4'
//}

問題に戻る: @<hd>{マージをやってみる}
