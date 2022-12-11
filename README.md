# CV関連の個人コード

## ビルド方法

CMakeを使用して以下のコマンドを使用することで、`build/bin/`ディレクトリに実行ファイルが生成される。

```s
cd kamecv
cmake -B build -S .
cmake --build build
```

自動テストを行う場合には、`build`ディレクトリに移動して以下のコマンドを使用する。

```s
cd build
make test
```
