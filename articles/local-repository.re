= ローカルリポジトリの操作

== 基本的な用語の説明

gitではチェックインする（履歴を登録する）前に、チェックインするファイルを選択することができます。
この選択したファイルが記録される場所のことを、インデックスまたはステージングエリアといいます。
インデックスの仕組みによって、記録したいファイルのみを選択してチェックインすることができます。

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

@<code>{git status}コマンドでリポジトリの状態が分かります。

//cmd{
$ git status
On branch main

No commits yet

Untracked files:
  (use "git add <file>..." to include in what will be committed)
        test.txt

nothing added to commit but untracked files present (use "git add" to track)
//}

== コミットグラフとは

== ワーキングツリーとインデックス

== コミットとブランチ

== マージ、2種類のマージ

== リベース
