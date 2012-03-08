= Red5でアプリケーションを作る

この文書ではRed5での新しいアプリケーションの作り方を説明する。Red5の0.4で導入された新APIにも触れる。

== アプリケーションのディレクトリ

Red5はアプリケーションをRed5のルート直下にあるwebappsディレクトリに保持する。
だからアプリ開発で最初にするのはwebappsの下にフォルダを作ることだ。
慣習として、このフォルダにはアプリケーションと同じ名前をつける。

新しいアプリケーションにはWEB-INFフォルダが必要で、そこには設定ファイルが置かれる。
doc/templates/myappフォルダにあるテンプレートが使える。

Red5は起動時にwebappsにある全てのフォルダのWEB-INFを探索する。

== 設定

設定の中心はweb.xmlファイルで、次のパラメータを含んでいる：

=== webAppRootKey

//emlist{
<context-param> 
    <param-name>webAppRootKey</param-name> 
    <param-value>/myapp</param-value> 
</context-param>
//}

== ハンドラの設定

ハンドラの設定ファイルには最低３つのbean定義が要る。

=== Context
コンテキストbeanはweb.contextという予約名があり、パスをスコープ、検索サービス、ハンドラに対応付けるのに使う。
デフォルトの定義は
//emlist{
<bean id="web.context" class="org.red5.server.Context" autowire="byType" />
//}
ひとつのアプリケーションはコンテキストを1つだけ持つ。

=== Scopes

すべてのアプリケーションは最低ひとつのスコープを持ち、スコープはハンドラをコンテキストとサーバに結びつける。
スコープはツリーを形成し、クライアントはその中のノードに接続してスコープ内でオブジェクトを共有する（共有オブジェクトやライブストリーム）。
スコープは部屋とかインスタンスとみなすことができる。

デフォルトのスコープは通常web.scopeという名前だが、これは変更できる。

スコープのbeanは次のパラメータを持つ：

: server
    red5のサーバ
: parent
    親スコープ。たいていはglobal.scope。
: context
    このスコープのコンテキスト。上のweb.contextを使え。
: handler
    このスコープのハンドラ。下を見ろ。
: contextPath
    このスコープに接続するためのパス。
: virtualHosts
    このスコープが動作する、コンマ区切りのホスト名またはIPアドレス。

//emlist{
<bean id="web.scope" class="org.red5.server.WebScope" 
    init-method="register"> 
    <property name="server" ref="red5.server" /> 
    <property name="parent" ref="global.scope" /> 
    <property name="context" ref="web.context" /> 
    <property name="handler" ref="web.handler" /> 
    <property name="contextPath" value="/myapp" /> 
    <property name="virtualHosts" value="localhost, 127.0.0.1" /> 
</bean> 
//}

contextPathとvirtualHostsは別のプロパティファイルに書くこともできて、その場合はもう一つbeanが要る：
//emlist{
<bean id="placeholderConfig" class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
    <property name="location" value="/WEB-INF/red5-web.properties" /> 
</bean> 
//}

red5-web.propertiesの中身が
//emlist{
webapp.contextPath=/myapp 
webapp.virtualHosts=localhost, 127.0.0.1 
//}
だとすると、スコープのプロパティは次のように書ける。
//emlist{
<property name="contextPath" value="${webapp.contextPath}" /> 
<property name="virtualHosts" value="${webapp.virtualHosts}" />     
//}
設定で指定されたcontextPathはスコープの「ルート」と考えるとよい。

設定されたパス以下に要素を付け足すと、スコープが動的に作成され、それに接続できる。

こうした追加スコープはすべて同じハンドラを使用するが、プロパティ、共有オブジェクト、ライブストリームは独自のものを持つ。

== ハンドラ

コンテクストはすべて１つのハンドラが必要で、ハンドラはクライアントがスコープに接続・切断したときに呼び出されるメソッドを実装し、また、クライアントから呼ばれるメソッドを持っている。

サンプル実装がorg.red5.server.adapter.ApplicationAdapterにある。
くわしくはjavadocをどうぞ。

スコープハンドラのbeanは次のようにして設定する。
//emlist{
<bean id="web.handler" class="the.path.to.my.Application" singleton="true" /> 
//}


== ロギング


