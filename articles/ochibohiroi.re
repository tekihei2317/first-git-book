= 落ち穂拾い

== ghqとpecoを使ったリポジトリの管理

複数のリポジトリをクローンすると、リポジトリをクローンする場所に迷ったり、リポジトリを探すのが手間になったりします。
ここではghqとpecoを使った解決策を説明します。

=== ghqとは

ghq@<fn>{ghq}は、ローカルにクローンしたリポジトリをシンプルに管理するためのツールです。
@<code>{ghq get <リポジトリURL>}でリモートリポジトリをクローンしたり、@<code>{ghq list}でクローンしたリポジトリの一覧を確認できます。

//cmd{
$ ghq list
github.com/isomorphic-git/isomorphic-git
github.com/stoplightio/sample-specs
github.com/tekihei2317/dotfiles
github.com/tekihei2317/git-problems
github.com/tekihei2317/git-problems_branching
github.com/tekihei2317/git-problems_commit
github.com/tekihei2317/git-problems_ff-merge
//}

//footnote[ghq][@<href>{https://github.com/x-motemen/ghq}]

=== pecoとは

peco@<fn>{peco}は、コマンドの出力をインタラクティブに絞り込むためのツールです。
標準出力の内容をpecoコマンドに渡すと、検索画面が起動します。
検索画面で文字を入力すると行が絞り込まれ、Enterで選択した行がpecoコマンドの標準出力になります。

//footnote[peco][@<href>{https://github.com/peco/peco}]

=== ghqとpecoを実際に使ってみる

Mac + zshを前提に書いているので、異なる環境の場合は適宜読み替えてください。

まず、ghqとpecoをHomebrewでインストールします。

//cmd{
brew install ghq
brew install peco
//}

次に、ghqでリポジトリをクローンします。tekihei2317/git-problemsをクローンしてみます。

//cmd{
$ ghq get https://github.com/tekihei2317/git-problems
//}

GitHubのリポジトリは@<code>{<ghqのルートディレクトリ>/github.com/<user>/<repository>}にクローンされます。
ghqのルートディレクトリは、デフォルトでは@<code>{~/ghq}です。@<code>{ghq root}で確認できます。

@<code>{ghq list}でリポジトリ一覧を表示し、pecoに渡します。

//cmd{
$ ghq list | peco
//}

まだ1行しかないので分かりにくいですが、文字を入力するとリポジトリが絞り込まれます。
Enterを押すと、選択した行が標準出力に表示されます。

//cmd{
$ ghq list | peco
github.com/tekihei2317/git-problems
//}

=== シェル関数を作成する

ghqとpecoを使用して、以下の2つの便利関数を作ってみます。

 * 選択したリポジトリのディレクトリに移動する
 * 選択したリポジトリをVSCodeで開く

まずは1つ目の関数です。名前はpeco + cd = pcdにしました。

//list[ghq-peco-pcd][選択したリポジトリに移動するコマンド]{
function pcd() {
  local repository=$(ghq list | peco)
  local directory="$(ghq root)/$repository"
  print -z "cd $directory"
}
//}

@<code>{print}はzshの組み込み関数で、-zオプションをつけると、渡した文字列を編集バッファ@<fn>{editing-buffer}に入れてくれます。
上記の関数を~/.zshrcに記入したあと、source ~/.zshrcで再読み込みしてください。

//footnote[editing-buffer][ターミナルでコマンドを入力する場所のことです。]

@<code>{pcd}と入力すると、pecoが起動します。
Enterを一度押すとcdコマンドが編集バッファに入り、もう一度Enterを押すと目的のディレクトリに移動できます。

//cmd{
$ pcd
$ cd /Users/{user}/ghq/github.com/tekihei2317/git-problems
$ pwd
/Users/{user}/ghq/github.com/tekihei2317/git-problems
//}

次はリポジトリをVSCodeで開く関数です。名前はpeco + code = pcodeにしました。実装はpcdコマンドとほとんど同じです。

//list[ghq-peco-pcode][選択したリポジトリをVSCodeで開くコマンド]{
function pcode() {
  local repository=$(ghq list | peco)
  local directory="$(ghq root)/$repository"
  print -z "code $directory -r"
}
//}

codeコマンドに-rオプションをつけて、現在のウィンドウでリポジトリを開くようにしています。
別のウィンドウで開くのが好みの場合は、-rオプションを削除してください。

pcodeコマンドを実行してリポジトリをEnterで選択し、再度Enterをクリックすると、目的のリポジトリを
VSCodeで開けます。

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

== これまでのまとめ

最後に、これまで学んできたgitコマンドをかんたんな図で振り返ります。

=== ローカルリポジトリの操作（2章）

gitにはワーキングツリー、インデックス、コミットの3つの状態があります。
3つとも、管理しているファイルのすべての状態をもったスナップショットです。

addコマンドでインデックスにステージしてから、commitコマンドでコミットを作成します。
作成したコミットは、直前に作成したコミット（親コミット）への参照を持ちます。

=== リモートリポジトリの操作（3章）

pushはブランチをリモートリポジトリにアップロードするコマンド、
pullはリモートリポジトリの状態を取得して、現在のブランチにマージするコマンドです。
fetchはリモートリポジトリの状態を取得するコマンドです。

リモートリポジトリのの状態を取得すると、リモート追跡ブランチが作成されます。
また、pushとpullの引数は、上流ブランチを設定すると省略できます。

=== ワーキングツリーとインデックスの操作（5章）

checkoutコマンドとresetコマンドは、複数の使い方があるやや複雑なコマンドです。

checkoutコマンドにパスを指定すると、インデックスでワーキングツリーが上書きされます（addしていない作業内容の取り消し）。
resetコマンドにパスを指定すると、HEADの内容でインデックスが上書きされます（addの取り消し）。

checkoutコマンドにコミットとパスを指定すると、指定したコミットのパスの状態がインデックスとワーキングツリーにコピーされます。
resetコマンドにコミットを指定すると、ブランチの参照先が変わるとともに、インデックスとワーキングツリーの状態をコミットに合わせられます。
