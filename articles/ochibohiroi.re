= 落ち穂拾い

== ghqとpecoを使ったリポジトリの管理

== checkoutコマンドの3つの使い方

ブランチの切り替えに使ってきた@<code>{git checkout}コマンドですが、実は3つの使い方があります。

=== HEADを移動する

HEADとは、現在の作業位置を表すポインタ（参照）です。HEADはブランチまたはコミットを指します。
HEADの実体は@<code>{.git/HEAD}です。gitは、HEADの内容から現在のブランチを判定します。

//cmd{
$ cat .git/HEAD
ref: refs/heads/main # 現在のブランチはmain
//}

@<code>{git checkout}の以下の2つの使い方は、HEADを移動させるコマンドです。

//list[git-checkout-move-head][checkoutでHEADを動かす]{
git checkout <branch>
git checkout <commit>
//}

checkoutコマンドでコミットを指定すると、detached HEAD状態になります。
detached HEADとは、HEADがブランチではなくコミットを指している状態のことです。

=== ステージしていない変更を削除する

2つ目の使い方は、ステージしていないワーキングツリーの変更を削除することです。

//list[git-checkout-delete-unstaged-changes][checkoutでステージしていない変更を削除する]{
git checkout <pathspec>
//}

上記のコマンドを実行すると、インデックスの内容でワーキングツリーが上書きされます。
つまり、ワーキングツリーのステージしていない変更が削除されます。

=== 過去のファイルの状態を取り出す

3つ目の使い方は、過去のファイルの状態を取り出すことです。
この使い方が、バージョン管理システムでの本来のチェックアウトの意味です。

指定したコミットの状態で、インデックスとワーキングツリーの状態を上書きします。

//list[git-checkout-checkout-files-from-commit][checkoutで過去のファイルの状態を取り出す]{
git checkout <commit> [--] <pathspec>
//}

== resetコマンド

== gitチートシート
