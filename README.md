# bve-autopilot

これは Bve trainsim 5 で自動運転を行うための保安装置プラグインです。特定の路線・車両での動作を忠実に再現することよりも、どの路線・車両にも後付けで組み込んで自動運転化できるようなものを目指しています。動作させるには路線・車両ともにそれなりの準備 (改造) が必要です。

## 機能一覧

- TASC
  - [x] 駅の所定位置への停車
  - [x] 地上子からの停止位置の設定
    - [x] 独自地上子
    - [x] 汎用 ATS プラグインの地上子
    - [x] δ ナックニスタさんの TASC プラグインの地上子
    - [ ] 小田急線向け ATS プラグインの地上子
  - [x] 計器パネルへの動作状態の表示
    - [x] ブレーキノッチ
    - [x] 停止位置までの残距離
  - [x] オン/オフ切り替え時に音を鳴らす
- ATO
  - [x] 速度の自動制御
    - [x] 独自地上子による制限速度の設定
    - [x] 信号による速度制御 (ATC でなくても可)
    - [x] 前方の信号の現示を地上子から受信
    - [x] 目標時刻に合わせて速度を調整
  - [x] メトロ総合プラグイン互換モード
    - [x] 前方予告に基づく速度制御
    - [x] ORP に沿う速度制御
  - [x] ATS-SW/P2 プラグイン互換モード (全線 P 路線用)
    - [x] 前方の信号に基づく速度制御
    - [x] 速度照査地上子に基づく速度制御
  - [x] 計器パネルへの動作状態の表示
    - [x] 力行・ブレーキノッチ
    - [x] 現在の制限速度
  - [x] オン/オフ切り替え時に音を鳴らす
- その他
  - [x] 独自地上子による勾配補正
  - [x] 自動ブレーキのノッチ段数を手動ブレーキのノッチ段数より多くする設定

## 動作環境・開発環境

Bve trainsim 5.7.6224.40815 で動作確認しています。BVE4 や OpenBVE のことは知りません。DetailManager からの使用も可能ですが、他の保安装置プラグインとの同時使用については動作未確認です。

このプラグインを修正・改造する場合は最新の Visual Studio の使用を推奨します。

### 非対応車両

- モーターの抵抗制御が手動進段式の車両は非対応です。
- 自動空気ブレーキの車両は非対応です。

### 向かない車両

- ブレーキの応答が遅い車両では停止位置がずれやすくなります。
- 応荷重制御のない車両ではブレーキの制御が不安定になります。
- 電空協調制御のない電気ブレーキを搭載する車両ではブレーキの制御が不安定になります。
- 加速度が 5 km/h/s 以上の車両は加速をやめるタイミングの制御が不安定になります。

## 使用法

プラグイン本体は[リリースページ](https://github.com/magicant/bve-autopilot/releases)からダウンロードできます。また[テスト用シナリオ](https://github.com/magicant/bve-autopilot-scenario)にも含まれています。

自動運転を正常に動作させるためには車両と路線の両方のデータを改造する必要があります。詳しい説明は [wiki](https://github.com/magicant/bve-autopilot/wiki) を見てください。また、地上子の設置方法や車両の設定ファイルの書き方の実例は[テスト用シナリオ](https://github.com/magicant/bve-autopilot-scenario)を参考にしてください。

### モード切替

[所定のキー操作](https://github.com/magicant/bve-autopilot/wiki/キー操作仕様)によって「ATO と TASC 両方有効」「TASC のみ有効」「どちらも無効」の三つのモードを切り替えることができます。

### TASC の操作方法

モード切替で TASC が有効になっていて、地上子から停止目標位置が受信できていれば、列車を走らせるだけで自動的に TASC のブレーキが動作します。停止目標位置付近に停車すると自動的にブレーキがかかったままになります。手動で自動ブレーキ以上のブレーキをかけるかドアが閉まると自動ブレーキは解除されます。(設定ファイルによりブレーキ解除条件は変えられます)

### ATO の操作方法

モード切替で ATO が有効になっていると、停車中は常に自動でブレーキがかかったままになります。ドアが閉まったら、レバーサーを前進にし手動ブレーキを緩めた状態で発進ボタンを押すと自動ブレーキが解除され列車が出発します。停止信号や手動ブレーキで列車が駅の手前に停止した場合も自動でブレーキがかかります。その場合も (信号が停止以外に変わってから) 発進ボタンを押すと再発進します。

## デモ動画

ブレーキのかけ方などは頻繁に改良を行っているので古い動画でのプラグインの動きは最新版とは異なることがあります。

- [東京メトロ副都心線 小竹向原→渋谷](https://youtu.be/6kXDLosXjx8) - v1.4 で撮影
- [阪和線・関西空港線 関空快速 天王寺→関西空港](https://youtu.be/92c_utzGNos) - v0.9 で撮影
- [東急東横線・みなとみらい線 渋谷→元町・中華街](https://youtu.be/y3LwPA8is3I) - v0.6 で撮影
- [信号試験線](https://youtu.be/ywAtS5WGzSg) - v0.5 で撮影
- [下り勾配試験線](https://youtu.be/sfnf7ql6HGw) - v0.4 で撮影
- [京成千葉線](https://youtu.be/tCQxyIPJYbs) - v0.2 で撮影

## 使用条件

このプラグインの使用条件は [LGPL 2.1](LICENSE) によります。つまり簡単に言うと

* プラグインとして車両データに組み込んで使うのは自由です。
* あなたがこのプラグインを修正・改造したものを公表しようとする場合、バイナリー (dll ファイル) だけでなくソースコードも公開する必要があります。そして公開されたものもまた LGPL に従わなければなりません。
  * せっかく GitHub で開発しているのだから役に立ちそうな修正はプルリクしてください。

## 解説

[algorithm.md](algorithm.md) ファイルにアルゴリズムの解説を書きました。

![bve-autopilot クラス図](https://docs.google.com/drawings/d/e/2PACX-1vS04CQzvJoFkLqhHuT_zZZW-YbpnRhUtPCcCVHPN-CMPTM_-LAcdUqEHE2YRKOrQcYnIqTFau8X7KWR/pub?w=960&h=720)

## 質問・問い合わせ

- [ツイッター](https://twitter.com/tnacigam)で
- [メール](mailto:magicant@wonderwand.net)で
- [Issues](https://github.com/magicant/bve-autopilot/issues/new) で
