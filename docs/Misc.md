# Misc

## ToDo

* Trace/Overhead計測ツール実装(大体OK)
* run_tq等を構造体のメンバ変数にすることで，モノリシックカーネルで
   キャッシュライン毎にアクセスできるようにする．
* Mcube KernelをLinuxに適用することを検討する．
* マルチカーネルの（メモリ共有）プロセス生成，破棄するcfork/cexit関数を実装する．
* Barrelfish/DCと同様にカーネルの更新にも応用可能．
   Microkernelにすれば特定のカーネルのみ更新すれば良い．
* x86でsyscall/sysretを利用したシステムコール実装(現在はint命令)
* x86でuser levelで動作
* ユーザレベルからカーネルレベルに移行するシステムコールを実装する．ARMは実装完了．
* Completely Fair Schedulerを実装
* Message Passingを作成
* simアーキテクチャでスケジューリングシミュレータを実装
* vsprintをvsnprintに変更

## Known Issues

* AXISで\verb|-fPIC|オプションを入れるとコンパイルエラーに
    なるので，利用しないことで対処している．
* QEMUではHPETが正しく動作しない．VMWareだと1回発生するだけ．

## Coding Styles

* 移植性はできる限り高く
* マクロは基本大文字にする
* プログラム中に参考文献の論文を書く
* \verb|a != NULL -> a|, \verb|a == NULL -> !a|
* 改行する場合，演算子(\verb|+-*/|等)は行の先頭に書く
* Doxygenで変数は@a，予約語は@cを利用
* グローバル変数は大文字から始める
