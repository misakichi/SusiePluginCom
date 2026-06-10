# SusiePluginCom
this is susie plugin com wrapper.

# registcom.bat の使い方

`registcom.bat` は、SusiePluginCom の **COM 登録（Install）／解除（Uninstall）** を行うためのバッチファイルです。  
バッチを実行すると、操作メニューが表示されます。

---

## 1. 実行方法

エクスプローラーで `registcom.bat` をダブルクリックするか、  
コマンドラインから次のように実行します。
```
registcom.bat
```
実行すると次の選択肢が表示されます。
```
Install or Uninstall (I/U)
```

- **I** … COM を登録（Install）  
- **U** … COM を解除（Uninstall）

いずれかを入力して Enter を押してください。

---

## 2. SmartScreen によるブロックについて

Windows は、ダウンロードしたバッチファイルを初回実行時に  
**SmartScreen によりブロックする場合があります。**

その場合は以下の手順で解除できます。

### ● SmartScreen のブロック解除方法
1. `registcom.bat` を右クリック → **プロパティ**
2. 下部に「**ブロックの解除**」が表示されていればチェックを入れる
3. OK を押す

これで実行できるようになります。
