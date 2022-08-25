= 複数人での開発

== プルリクエストとは

プルリクエストとは、リモートリポジトリのあるブランチを別のブランチにマージするための依頼のことです。
プルリクエストは、GitHubなどのリモートリポジトリを管理するサービスにある機能です@<fn>{pull-request}。

//footnote[pull-request][GitLabではマージリクエストといいます。]

プルリクエストには、1つのリモートリポジトリで行うものと、リポジトリをフォーク@<fn>{github-fork}して行うものがあります。
ここでは実務で使うことが多い前者のプルリクエストについて説明します。
後者のプルリクエストは、OSSにコントリビュートする際に使われることが多いです。

//footnote[github-fork][リポジトリを自分のアカウントにコピーすることをいいます。@<href>{https://docs.github.com/ja/get-started/quickstart/fork-a-repo}]

== プルリクエストをやってみよう

プルリクエストの作成から反映までの手順は以下のようになります。
かっこの中は対応するgitコマンドです。

 * リモートリポジトリに変更を送る（push）
 * GitHub上でプルリクエストを作成する
 * プルリクエストをマージする（merge）
 * ローカルリポジトリに変更を反映する（fetch + merge）

それでは順番にやってみましょう。

=== リモートリポジトリに変更を送る

まず新しくリポジトリを作成します。今回は手順を減らすために、リモートリポジトリを先に作ります。

GitHubにアクセスし、ヘッダーの+ボタンからNew repositoryをクリックします。
リポジトリ名にpull-request-practiceと入力し、Add a README fileにチェックを入れて作成します。
こうすると、README.mdがコミットされた状態でリポジトリを作成できます。

//image[create-repository-on-github][GitHubでリポジトリを作成する]{
//}

リモートリポジトリを作成したら、クローンしてローカルリポジトリを作成します。

//cmd{
$ git clone git@github.com:<ユーザー名>/pull-request-practice.git
$ cd pull-request-practice
//}

新たにブランチを作成し、READMEに変更を加えてコミットします。
コミットしたらリモートリポジトリにプッシュします。

//cmd{
$ git checkout -b practice
$ echo '\n\nプルリクエストの練習です' >> README.md
$ git add . && git commit -m 'READMEにリポジトリの説明を追記'
$ git push -u origin practice
//}

=== プルリクエストを作成する

GitHubのリポジトリページを見ると、practiceブランチがプッシュされたという通知が表示されています。

//image[branch-push-notification][ブランチのプッシュの通知]{
//}

通知のCompare & pull requestをクリックすると、プルリクエスト作成ページが開きます。
そのままCreate pull requestをクリックし、プルリクエストを作成します。

//image[pull-request-page][プルリクエストの作成が完了]{
//}

=== プルリクエストをマージする

プルリクエストが作成できたら、Merge pull requestをクリックしてマージしましょう。
実際の開発では、プルリクエストのFiles changedページから差分を確認したり、コードレビューをしたりします。

=== ローカルリポジトリに変更を反映する

プルリクエストをマージしたことで、READMEへの変更がmainブランチに反映できました。

//image[after-pr-merged][マージ完了後のmainブランチ]{
//}

「3 commits」となっていることから、マージコミットが作られたことが分かります。
つまり、GitHubでのプルリクエストのマージはNon Fast-forwardマージです。

では、この変更をローカルリポジトリに反映します。
現在のリポジトリの状態を整理すると、以下のようになっています。

//image[commit-graph-before-merge][現在のリポジトリの状態]{
//}

ローカルリポジトリに反映するには、前章でやったように@<code>{git fetch}でリモートリポジトリの状態をダウンロードしてから、
@<code>{git merge}でリモート追跡ブランチをマージします。

//cmd{
$ git fetch
$ git checkout main
$ git merge origin/main
//}

早送りマージが行われ、mainブランチがリモートリポジトリのmainブランチと同じ状態になりました。

//image[git-graph-after-pr-merged][ローカルリポジトリへの反映が完了]{
//}

== 3方向マージとコンフリクト

=== コンフリクトとは

コンフリクトとは、gitがマージを自動で行えないことをいいます。コンフリクトは、マージする2つのコミットで、
同じファイルの同じ箇所を変更したり、片方のコミットで削除したファイルをもう一方で変更したときに発生します。

コンフリクトが起きる状況について正しく理解するためには、gitのマージの仕組みの理解が必要です。

=== gitのマージの仕組み

gitのマージは、取り込まれるコミット・取り込むコミット・それらの共通祖先@<fn>{merge-base}の3つコミットを比較して行われます。
このように、3つの状態を比較してマージすることを3方向マージといいます。

//image[three-way-merge][3方向マージ]{
//}

//footnote[merge-base][この共通祖先のことをマージベースといいます。コミットAとBのマージベースは、@<code>{git merge-base A B}で分かります。]

3方向マージの実装について考えてみましょう。あるファイルについて、片方のコミットでのみ追加・編集・削除が行われていた場合は、その変更を採用すればよいです。
問題は、両方のコミットが同じファイルを変更している場合です。これは、以下の4つに場合分けできます@<fn>{why-add-edit-case-does-not-exist}。

//table[git-merge-modify-same-file][同じファイルを変更している場合]{
コミットA	コミットB	対応方法
------------
追加	追加	ファイルの3つの状態を比較してマージする
編集	編集	ファイルの3つの状態を比較してマージする
編集	削除	コンフリクトを発生させる
削除	削除	ファイルを削除する
//}

3番目の(編集, 削除)のケースは、gitにはどちらを優先すればよいか分からないため、コンフリクトが発生します。
(追加, 追加)と(編集, 編集)のケースは、gitはそのファイルの3つのコミットでの状態を比較して、可能な限りマージします。
マージできなかった場合は、マージできなかった箇所がコンフリクトとしてマークされます。

//footnote[why-add-edit-case-does-not-exist][(追加, 編集)がないのは、Aで追加した場合は共通祖先にそのファイルが存在しないため、Bで編集するのはおかしいからです。(追加, 削除)がないのも同様の理由です。]

== コンフリクトが起きる場合、起きない場合

同じファイルを編集しても、コンフリクトが起きる場合と起きない場合があります。
なぜかというと、コンフリクトが発生するのは「マージする2つのコミットで、同じ箇所に別の変更がされている」と判定されたときだからです。

この「箇所」というのは、gitのdiffのアルゴリズムによって決まります。具体的な例で見てみましょう。

=== コンフリクトが起きない場合

//list[original][もとのファイル][javascript]{
function add(x, y) {
  return x + y;
}
//}

//list[feature1][派生ファイル1][javascript]{
function minus(x, y) {
  return x - y;
}

function add(x, y) {
  return x + y;
}
//}

//list[feature2][派生ファイル2][javascript]{
function add(x, y) {
  return x + y;
}

function multiply(x, y) {
  return x * y;
}
//}

派生ファイル同士をマージすると、以下のようになります。

//list[merged][マージしたファイル][javascript]{
function minus(x, y) {
  return x - y;
}

function add(x, y) {
  return x + y;
}

function multiply(x, y) {
  return x * y;
}
//}

2つのファイルが、それぞれadd関数の上と下という「別の箇所」を変更したと判定されるため、
マージが成功します。

=== コンフリクトが起きる場合

//list[original_case1][もとのファイル][javascript]{
function add(x, y) {
  return x + y;
}
//}

//list[feature1_case1][派生ファイル1][javascript]{
function add(x, y) {
  return x + y;
}

function minus(x, y) {
  return x - y;
}
//}

//list[feature2_case1][派生ファイル2][javascript]{
function add(x, y) {
  return x + y;
}

function multiply(x, y) {
  return x * y;
}
//}

add関数の下という「同じ箇所」に別の変更がされていると判定されるため、コンフリクトします。

== コンフリクトの解消方法

それでは、実際にコンフリクトの解消をやってみましょう。
先述のコンフリクトの例を使います。
featureブランチでminus関数を追加したものの、
mainブランチにmultiply関数が追加されてコンフリクトしてしまったという想定です。

//image[git-graph-before-merge][コンフリクトしたときの状況]{
//}

まずは、以下のリポジトリをフォークして、自分のアカウントにリポジトリを作成して下さい。
フォークするには、リポジトリのページ上部のForkボタンをクリックします。

@<href>{https://github.com/tekihei2317/git-problems_resolve-conflict}

フォークする際は、Copy the main branch onlyのチェックを外してすべてのブランチをコピーします。

//image[uncheck-copy-the-main-branch-only][チェックを外してすべてのブランチをコピーする]{
//}

リポジトリが作成できたら、featureブランチからmainブランチへのプルリクエストを作成します。
Pull Request→New Requestから作成します。
base repository（プルリクエストの送り先）がデフォルトではフォーク元のリポジトリになっているため、フォークしたリポジトリに切り替えます。

//image[change-base-repository][ベースリポジトリを変更する]{
//}

プルリクエストを作成すると、コンフリクトが発生してプルリクエストがマージできないことが分かります。

//image[conflict-occur][コンフリクトが発生]{
//}

それではコンフリクトを解消してみましょう。
GitHub上でもコンフリクトを解消できますが、練習のためにローカルのエディタで解消します。

ローカルでのコンフリクトの解消は、以下の手順で行います。

 * マージしたいブランチに、マージ先のブランチをマージする
 * コンフリクトを解消する

まず、プルリクエストでマージしたいブランチ（feature）に移動し、マージ先のブランチ（main）をマージします。
こうすることで、コンフリクトが発生した箇所がファイルにマークされます。

//cmd{
$ git checkout feature
$ git merge main
Auto-merging index.js
CONFLICT (content): Merge conflict in index.js
Automatic merge failed; fix conflicts and then commit the result.
//}

//list[conflicted-file][コンフリクトしたファイル]{
function add(x, y) {
  return x + y;
}

<<<<<<< HEAD
function minus(x, y) {
  return x - y;
=======
function multiply(x, y) {
  return x * y;
>>>>>>> main
}
//}

イコールで区切られた上の部分が現在のブランチの内容で、下の部分がマージしたブランチの内容です。

次に、コンフリクトを解消します。両方の関数を追加したいので、ファイルを編集して両方の変更を残します。

//list[fix-conflicted-file][コンフリクトを修正したファイル]{
function add(x, y) {
  return x + y;
}

function minus(x, y) {
  return x - y;
}

function multiply(x, y) {
  return x * y;
}
//}

コンフリクトを解消したらコミットします。
コミットメッセージは自動で入力されるため、それを使用することにします。

//cmd{
$ git add . && git commit
$ git push
//}

コミットしたらプッシュして、プルリクエストを確認します。
マージできるようになっているので、マージすれば完了です。

//image[github-merge-conflict-resolved][コンフリクトを解消するとマージできる]{
//}

== ブランチ戦略

ブランチ戦略@<fn>{branch-strategy}とは、gitのブランチ機能をどのように使用するかを決めたルールのことです。
具体的には、いつどのようなブランチを作成し、どのようにマージし、デプロイ・リリースするかを決めたものです。
代表的なブランチ戦略には、Git Flow@<fn>{git-flow}、GitHub Flow@<fn>{github-flow}、Git Feature Flow@<fn>{git-feature-flow}などがあります。

ここでは、プルリクエストを使った最もシンプルなブランチ戦略であるGitHub Flowを説明します。

//footnote[branch-strategy][ブランチモデルともいいます。]
//footnote[git-flow][@<href>{https://nvie.com/posts/a-successful-git-branching-model}]
//footnote[github-flow][@<href>{https://gist.github.com/Gab-km/3705015}]
//footnote[git-feature-flow][@<href>{https://developers.gnavi.co.jp/entry/GitFeatureFlow/koyama}]

=== GitHub Flowとは

GitHub Flowは、初期のGitHubの開発で使われていたシンプルなブランチ戦略です。
mainブランチから作業内容を表す名前のブランチを作成し、そのブランチをプルリクエストを使ってmainブランチにマージします。

より詳細には、以下のようなルールがあります。

 * mainブランチは常にデプロイ可能にする
 * プルリクエストをレビューしてもらってOKをもらったら、mainブランチにマージ可能になる
 * mainブランチにマージしたら、ただちにデプロイする

GitHub Flowでは、mainブランチを常にデプロイ可能にすることが、最も厳格な（守るべき）ルールであるとされています。
なぜかというと、このルールが頻繁なデプロイを促進するからです。
そうすることで、新機能の追加や機能の修正を素早く行えるようになります。
