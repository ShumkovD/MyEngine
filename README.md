# MyEngine
紹介動画：　https://youtu.be/YVKLjM9FT9g
プロジェクトに *submodule* が入っているために、ダウンロードする際 *git clone --recursive* を使って下さい。

実行すると *ARM* ではなく *x64* か *x86* を選んでください。
このプロジェクトは**コミット**や**エラー**は**英語**ですが、**コメント**は**日本語**で書いてあります。

このプロジェクトは10月上旬から３Dプログラミングとｃ++知識を深めるために始めました。途中でシェーダーを作るため、HLSLの基礎知識を学習しました。そしてWindowsアプリケーションを作れるようになりました。
>デモでの操作：
- ESC　－　プログラム終了
- ←　 －　モデル拡大
- →　 －　モデル収縮
- W   －　カメラを前に移動します
- S　 －　カメラを後ろに移動します
- Q　E －　ロカルY回転	    「軸はモデル中心」
- A　S －　グロバルモデルY回転「軸は世界の中心」

コードは *Engine11* フォルダに入っています。

> 今対応している機能：
- スプライトは様々な拡張子を対応しています「DirectXTK」、
- 透明度とピクセルクリッピングの対応、
- ２Dレンダー「DirectXTKのSpriteBatch使用」、
- FPSタイマー
- 簡単な入力、ライトニング、
- obj.　拡張子のモデルの対応「DirectXTK」、
  
> 未来には開発の予定：
- vector にテクスチャーやモデルの情報をまとめること。
- コードをクラスに分けること。
- bumpマッピング

参考：
- https://www.braynzarsoft.net/viewtutorial/q16390-braynzar-soft-directx-11-tutorials
- https://github.com/microsoft/DirectXTK/wiki
