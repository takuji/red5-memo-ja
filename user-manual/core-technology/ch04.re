= セキュリティ

ストリームや共有オブジェクトへのアクセスを保護するRed5 APIについて説明する。

== ストリームのセキュリティ
読み取り（再生）と書き込み（放送・録画）の保護は個別に行われる。

=== ストリーム再生のセキュリティ
ユーザごとにストリーム再生を制限したり、特定の名前のストリームだけにアクセスを許可したいアプリのために、IStreamPlaybackSecurityインターフェイスが提供されている。
http://dl.fancycode.com/red5/api/org/red5/server/api/stream/IStreamPlaybackSecurity.html

これはどのオブジェクトに実装してもよく、ApplicationAdapterに登録される。
ストリーム・セキュリティ・ハンドラは、ひとつのアプリに何個でも使える。
ハンドラのうち１つでもアクセスを拒否すると、クライアントはNetStream.Failedエラーを受け取り、そのdescriptionフィールドに該当するエラーメッセージが書いてある。

"liveStream"ではじまる名前のストリームにだけアクセスを許可するハンドラの例がこちら：
//emlist{
import org.red5.server.api.IScope; 
import org.red5.server.api.stream.IStreamPlaybackSecurity; 
            
public class NamePlaybackSecurity implements IStreamPlaybackSecurity { 
            
    public boolean isPlaybackAllowed(IScope scope, String name, int start, 
    int length, boolean flushPlaylist) { 
        if (!name.startswith("liveStream")) { 
            return false; 
        } else { 
            return true; 
        } 
    };             
} 
//}
このハンドラをアプリケーションに登録するにはappStartメソッドに次のように書いておく。
//emlist{
registerStreamPlaybackSecurity(new NamePlaybackSecurity()); 
//}
Red5にはサンプルとして全てのアクセスを拒否するセキュリティ・ハンドラが含まれている（DenyAllStreamAccess）。


=== ストリーム送信のセキュリティ

ユーザが公開したり録音することを許可するアプリケーションではたいてい、誤った使い方を防ぐためにアクセスを制限する必要がある。
Red5ではそのためにIStreamPublishSecurityインターフェイスが提供されている。

IStreamPlaybackSecurityと同じく、どのオブジェクトに実装できて、ApplicationAdapterに登録する。
ハンドラのうち１つでもアクセスを拒否すると、クライアントはNetStream.Failedエラーを受け取り、そのdescriptionフィールドに該当するエラーメッセージが書いてある。

認証された接続だけに"liveStream"ではじまる名前のライブストリームの公開を許可するハンドラの例がこちら：

//emlist{
例がない！
//}
