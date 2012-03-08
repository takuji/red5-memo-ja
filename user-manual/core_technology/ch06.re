= クラスタリング

Red5のクラスタリングの説明をする。

以下に説明するのと同じ設定をclusterフォルダに出力するビルドターゲットがbuild.xmlに含まれてる。
Red5クラスタリングの設定をするには'ant dist-cluster'を使う。

== 制限

今のところ一つのオリジン、複数のエッジというクラスタリング構成しかサポートしていない。
エッジサーバはRTMP接続しか受け付けない。

== サーバ設定

=== 設定ファイル

エッジ/オリジン構成をサポートするための設定ファイルがいくつか。

: red5-edge.xml, red5-edge-core.xml
    エッジのspring beanの設定に使う。conf以下に置く。
: red5-origin.xml, red5-origin-core.xml
    オリジンのspring beanの設定。conf以下に置く。

== エッジサーバの設定

エッジにはアプリケーションを配置する必要がない。

エッジはオリジンとは別のサーバに配置することを全力で推奨するけども、オリジンと同じサーバに置くことも可能っちゃ可能。

=== オリジンと別のサーバにエッジを配置する

red5-edge-core.xmlを編集して"mrtmpClient" beanがオリジンサーバを指すように設定する。
//emlist{
<bean id="mrtmpClient"
    class="org.red5.server.net.mrtmp.MRTMPClient" init-method="start" >
    <property name="ioHandler" ref="mrtmpHandler" />
    <property name="server" value="${mrtmp.host}" />
    <property name="port" value="${mrtmp.port}" />
</bean>
//}

red5.xmlをred5-edge.xmlに置き換えて、サーバを起動する。
//emlist{
./red5.sh

または

java \-jar red5.jar
//}

=== オリジンと同じサーバにエッジを配置する

red5.xmlはそのままで、red5-edge.xmlを$(RED5_ROOT)/confから$(RED5_ROOT)にコピーしてサーバを起動する。
//emlist{
java \-jar red5.jar red5-edge.xml
//}
あるいはred5.xmlを編集してred5-edge.xmlパラメータを追加して
//emlist{
./red5.sh
//}

=== オリジンサーバを設定する

webappsにアプリケーションを置く。
9035ポートがファイアウォールにブロックされていないことを確認する。
このポートはエッジがオリジンに接続するのに使われる。

red5.xmlをred5-origin.xmlで更新する。
サーバを起動する。
//emlist{
./red5.sh

または

java \-jar red5.jar
//}

== アプリケーションを使用する

RTMPのデータはもうエッジに流れている。
RTMPTとHTTPはいつも通りオリジンから流れているはず。