import socket, time

mode = True
while True:
    # ソケットを作成する
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        # 変数modeの値に合わせて送信する文字列を決める
        str = 'On' if mode else 'Off'
        print('Send ' + str)
        # サーバーに文字列を送信する
        s.connect(('192.168.11.1', 12345))
        s.sendall((str + '\n').encode())
        # レスポンスを受信する
        data = s.recv(1024)
        # レスポンスを表示する
        print('Recv ' + data.decode())
        # ソケットを閉じる
        s.close()
    # 1秒間待つ
    time.sleep(1)
    # オン／オフを反転する
    mode = not mode
