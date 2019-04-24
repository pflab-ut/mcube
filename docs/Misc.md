# Misc (in Japanese)

## ToDo

* run_tq等を構造体のメンバ変数にすることで，モノリシックカーネルで
   キャッシュライン毎にアクセスできるようにする．
* Mcube KernelをLinuxに適用することを検討する．
* マルチカーネルの（メモリ共有）プロセス生成，破棄するcfork/cexit関数を実装する．
* x86でsyscall/sysretを利用したシステムコールを実装する．
* x86でuser levelで動作する．
* Completely Fair Schedulerを実装する．
* Message Passingを作成する．
* simアーキテクチャでスケジューリングシミュレータを実装する．
* NR_CPUSを動的に設定する．
* AXISでメモリサイズを動的に設定する

## Known Issues

* AXISで`-fPIC`オプションを入れるとコンパイルエラーに
    なるので，利用しないことで対処している．
* QEMUではHPETが正しく動作しない．VMWareだと1回のみ発生する．

## Coding Styles

* 移植性はできる限り高くする．
* マクロは基本大文字にする．
* プログラム中に参考文献の論文を書く．
* `a != NULL -> a`, `a == NULL -> !a`に変更する．
* 改行する場合，演算子(+-*/等)は行の先頭に書く．
* Doxygenで変数は@a，予約語は@cを利用する．
* グローバル変数は大文字から始める．
