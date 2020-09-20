import socket

# ソケットを作成する
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    # 12345番ポートで接続を待つ
    s.bind(('192.168.11.11', 12345))
    s.listen(1)
    while True:
        # 接続と送信元IPアドレスを得る
        conn, addr = s.accept()
        with conn:
            while True:
                # データを受信する
                data = conn.recv(1024)
                if not data:
                    break
                data = data.decode()
                # 受信したデータを表示する
                print('Recv:' + data)
                # レスポンスを送信する
                conn.sendall(('Received: ' + data + '\n').encode())
