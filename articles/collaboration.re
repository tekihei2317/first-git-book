= 複数人での開発

== プルリクエストとは

プルリクエストとは、自分がリポジトリに加えた変更を、リモートリポジトリに反映してもらう依頼のことです。
プルリクエストはGitHubの機能です。

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

まず新しくリポジトリを作成します。今回はリモートリポジトリを先に作ってみます。
GitHubにアクセスし、ヘッダー+ボタンからNew repositoryをクリックします。
リポジトリ名はpull-request-practiceにし、Add a README fileにチェックを入れて作成します。

//image[create-repository-on-github][GitHubでリポジトリを作成する]{
//}

リモートリポジトリを作成したら、クローンしてローカルリポジトリを作成します。

//cmd{
$ git clone git@github.com:<ユーザー名>/pull-request-practice.git
$ cd pull-request-practice
//}

ブランチを作成し、READMEに変更を加えてコミットします。
コミットしたらリモートリポジトリにプッシュします。

//cmd{
$ git checkout -b practice
$ echo "\n\nプルリクエストの練習です" >> README.md
$ git add . && git commit -m 'READMEにリポジトリの説明を追記'
$ git push -u origin practice
//}

=== プルリクエストを作成する

GitHubでリポジトリを見ると、practiceブランチがpushされたという通知が表示されています。

//image[branch-push-notification][ブランチのpushの通知]{
//}

通知のCompare & pull requestをクリックし、プルリクエスト作成ページが開きます。
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
つまり、プルリクエストのマージはNon Fast-forwardマージです。

では、この変更をローカルリポジトリに反映してみます。
現在のリポジトリの状態を整理すると、以下のようになっています。

TODO: 図を入れる

まず、@<code>{git fetch}でリモートリポジトリの状態をダウンロードします。

//cmd{
$ git fetch
//}

TODO: 図を入れる

//cmd{
$ git checkout main
$ git merge origin/main
//}

早送りマージが行われ、mainブランチがリモートリポジトリのmainブランチと同じ状態になりました。

TODO: 図を入れる

== 3方向マージとコンフリクト

複数人で開発すると、コンフリクトが発生することがあります。
コンフリクトとはgitがマージを自動で行えないことです。
コンフリクトについて説明する前に、gitのマージについて掘り下げてみます。

== gitのマージの仕組み

gitのマージは、取り込まれるコミット・取り込むコミット・それらの共通祖先の3つコミットを比較して行われます。
このように、3つの状態を比較してマージすることを3方向マージといいます。

TODO: 図を入れる

同じファイルについて、片方のコミットでのみ追加・編集・削除が行われていた場合は、それを採用すればよいです。
問題は、両方のコミットが同じファイルを追加・編集していた場合です。
この場合、gitはdiff3のアルゴリズムを使って3つのファイルを比較・マージします。

== コンフリクトが起きる場合、起きない場合

同じファイルを編集しても、コンフリクトが起きない場合があります。
なぜかというと、コンフリクトが発生するのは「マージする2つのコミットで、同じ箇所に別の変更がされている」@<fn>{how-diff3-merge-work}と判定されたときだからです。
具体的な例で見てみましょう。

//footnote[how-diff3-merge-work][何件かで実験して得た考察のため、間違っている可能性があります。]

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

これらの3つのファイルをマージすると、以下のようになります。

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

2つの変更が、それぞれadd関数の上と下という「別の箇所」を変更したと判定されたため、
マージが成功しています。

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

TODO: 実際にやった結果をペーストする

//list[conflicted][コンフリクトしたファイル][javascript]{
function add(x, y) {
  return x + y;
}

<<<<<<< main
function multiply(x, y) {
  return x * y;
}
=======
function minus(x, y) {
  return x - y;
}
>>>>>>> feature
//}

add関数の下という同じ「箇所」に、別の変更がされているためコンフリクトしています。

== コンフリクトの解消方法

それでは、実際にコンフリクトの解消をやってみましょう。
先述のコンフリクトの例を使います。
featureブランチでminus関数を追加したものの、
mainブランチにmultiply関数が追加されてコンフリクトしてしまったという想定です。

まずは、以下のテンプレートリポジトリを使って、自分のアカウントにリポジトリを作成しましょう。
リポジトリを作成したら、ローカルにクローンします。

TODO: テンプレートリポジトリの画像を入れる

GitHubのプルリクエストの画面から、プルリクエストを作成します。
プルリクエストを作成すると、コンフリクトが発生してマージできないということが分かります。

TODO: 画像を入れる？

それではコンフリクトを解消してみましょう。
GitHub上でもコンフリクトを解消できますが、練習のためにローカルのエディタで解消します。

ローカルでのコンフリクトの解消は、以下のように行います。

 * マージするブランチに、マージ先のブランチをマージする
 * コンフリクトを解消する

まず、マージしたいブランチに移動し、マージ先のブランチをマージします。
こうすることで、コンフリクトが発生した箇所がファイルにマークされます。

TODO: コンフリクトの内容を書く

次に、コンフリクトを解消します。
イコールで区切られた上の部分が現在のブランチの内容で、下の部分がマージしたブランチの内容です。

両方の関数を追加したいので、両方の変更を残します。@<fn>{vscode-merge-editor}

//footnote[vscode-merge-editor][VSCode1.69(2022年6月版)からマージエディタが追加されたため、それを使用してもよいかもしれません。]

コンフリクトを解消したらコミットします。
コミットメッセージは自動で入力されるため、それを使用することにします。

//cmd{
$ git add . && git commit
//}

コミットしたらプッシュして、プルリクエストを確認します。
マージできるようになっているので、マージすれば完了です。

TODO: マージできるようになった画像を入れる？

== ブランチ戦略

ブランチ戦略@<fn>{branch-strategy}とは、gitのブランチ機能をどのように使用するかを決めたルールのことです。
具体的には、いつどのようなブランチを作成し、どのようにマージするかを決めます。
代表的なブランチ戦略には、Git Flow@<fn>{git-flow}、GitHub Flow@<fn>{github-flow}、Git Feature Flow@<fn>{git-feature-flow}などがあります。

ここでは、プルリクエストを使った最もシンプルなブランチ戦略であるGitHub Flowを説明します。

//footnote[branch-strategy][ブランチモデルともいいます。]
//footnote[git-flow][@<href>{https://nvie.com/posts/a-successful-git-branching-model}]
//footnote[github-flow][@<href>{https://gist.github.com/Gab-km/3705015}]
//footnote[git-feature-flow][@<href>{https://developers.gnavi.co.jp/entry/GitFeatureFlow/koyama}]

=== GitHub Flowとは

GitHub Flow@<fn>{github-flow}は、初期のGitHubで開発で使われていたシンプルなブランチ戦略です。
mainブランチから作業内容を表す名前のブランチを作成し、そのブランチをプルリクエストを使ってmainブランチにマージします。

TODO: 図を入れる

より詳細には、以下のようなルールがあります。

 * mainブランチは常にデプロイ可能にする
 * プルリクエストをレビューしてもらってOKをもらったら、mainブランチにマージ可能になる
 * mainブランチにマージしたら、ただちにデプロイする*

GitHub Flowでは、mainブランチを常にデプロイ可能にすることが、最も厳格な（守るべき）ルールであるとされています。
なぜかというと、このルールが頻繁なデプロイを促進するからです。
そうすることで、新機能の追加や機能の修正を素早く行えるようになります。
