本文では `timer_and_www.ino` について解説する。

他のファイルは技術検証用に作成したもので、本機能とは関係が無いので説明は省く。

# 機能
- 一定時間に一回ステッピングモーターを回転させる
- Web上に配置したボタンを押したときにステッピングモーターを回転させる

---

# 動作要件

- esp32(arduino互換マイクロコントローラ)
- ステッピングモーター
- wi-fi環境(2.4GHz)

---

# 導入手順

## install

- https://github.com/me-no-dev/ESPAsyncWebServer
- https://github.com/me-no-dev/AsyncTCP

上記からzipファイルをDL。

「スケッチ->ライブラリをインクルード->.zip形式のライブラリをインストール」よりDLしたzipファイルをそれぞれ読み込む。

## wi-fi

コード内の下記を編集。

```
const char *ssid      = "********"; // @todo: 各環境の設定
const char *password  = "********"; // @todo: 各環境の設定
IPAddress ip(192, 168, 1, 123);     // IPアドレス（本機が利用するIP）
IPAddress gateway(192, 168, 1, 1);  // デフォルトゲートウェイ
IPAddress subnet(255, 255, 255, 0); // サブネットマスク
```

ssid, passwordはwi-fi機器の設定に応じて変更、IPアドレスは所属しているネットワークアドレスに応じて変更する。

## ステッピングモーター

```
#define STEPS 2048 // ステッピングモーターのステップ数
#define MOTOR_RPM 10 // RMP設定
#define MOTOR_PIN1 13 // モーターのピン接続
#define MOTOR_PIN2 12
#define MOTOR_PIN3 14
#define MOTOR_PIN4 27
```

ステップ数、RPMはお好みで。
ピン番号は接続に応じて変更。

## タイマ割り込み設定

```
#define TIMER_DIVIDER 80 // ESP32は80MHzで動作
#define TIMER_ALARM_SEC 60 // タイマ割り込みを秒単位で設定
```

上記だと（ESP32 devkit v1の場合）1分間隔でモーターが回転。
例えば1日間隔にする場合は60x60x24=86400をTIMER_ALARM_SECに設定する。

---

# 起動～利用

Webサーバは起動までに時間がかかる（1～2分ぐらい）

http://192.168.1.123/ で画面中央にボタンのみ配置されたページが表示される。

ボタンを押すと http://192.168.1.123/request に遷移（thanksのみ表示するページ）し、モーターが回転する。

ボタンを押す動作無しに http://192.168.1.123/request に直接アクセスするだけでもモーターは回転する。

Webサーバを外部に公開するにはWi-Fiルータのポートフォワーディング機能を利用する。
