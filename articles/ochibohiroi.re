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

@<code>{git reset}コマンドは、2つの使い方があるコマンドです。
1つ目の使い方は、HEADやインデックス・ワーキングツリーを、指定したコミットの状態で更新します。
もう一つの使い方は、指定したコミットの状態でインデックスを更新します。

=== インデックスを更新する

@<code>{git reset}の1つ目の使い方は、指定したコミットでインデックスを更新することです。以下の書式を取ります。

//list[git-reset-update-index][resetでインデックスを更新する]{
git reset [<commit>] [--] <pathspec>
//}

コミットは省略可能で、省略した場合はHEADが使われます@<fn>{update-index-with-previous-commit}。

HEADでインデックスを更新するというのは、addを取り消すという意味になります。
つまり@<code>{git reset <pathspec>}は、@<code>{git add <pathspec>}と逆の操作をするコマンドです。

//footnote[update-index-with-previous-commit][コミットを指定してもワーキングツリーが更新されないため、代わりにgit checkout <commit> <pathspec>を使ったほうがよいでしょう。]

=== HEADとブランチを移動する

@<code>{git reset}の2つ目の使い方は、指定したコミットにHEADとカレントブランチを移動することです。書式は以下です。

//list[git-reset-update-head-and-ranch][resetでHEADとブランチを移動する]{
git reset [--soft | --mixed | --hard ] [<commit>]
//}

コミットは省略可能で、省略した場合はHEADが使われます。

オプションの違いは、インデックスとワーキングツリーを更新するかどうかです。
--hardはインデックスとワーキングツリーを更新します。--mixedはインデックスを更新します。--softはどちらも更新しません。
オプションを指定しなかった場合は--mixedが使われます。

=== 主な使い方

筆者は、以下の2つのコマンドをしばしば使います。

 * @<code>{git reset .}
 * @<code>{git reset --hard HEAD}

@<code>{git reset .}は、@<code>{git add .}と逆の意味のコマンドです。ステージした変更を取り消します。
@<code>{git reset --hard HEAD}@<fn>{reset-working-tree-using-checkout}はHEAD（最後のコミット）でインデックスとワーキングツリーを更新します。
最後のコミット以降に変更した内容を取り消したいときに使います。

//footnote[reset-working-tree-using-checkout][ステージする前に取り消すほうが多いため、たいていは@<code>{git checkout .}で間に合います。]

== gitチートシート
