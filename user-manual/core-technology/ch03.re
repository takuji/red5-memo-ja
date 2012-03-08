= ストリームのパスをカスタマイズする

この文書ではアプリケーションがビデオをオンデマンドで放映したり、デフォルトのストリームフォルダ以外の場所に録画する方法を説明する。

== ファイル名生成サービス

Red5では特定のスコープ用の機能を提供するものをスコープサービスと呼ぶ。
そのひとつがIStreamFilenameGeneratorで、再生・録画されるVODストリームのファイル名を生成する。
http://dl.fancycode.com/red5/api/org/red5/server/api/stream/IStreamFilenameGenerator.html

== カスタム・ジェネレータ

//emlist{
import org.red5.server.api.IScope; 
import org.red5.server.api.stream.IStreamFilenameGenerator; 

public class CustomFilenameGenerator implements IStreamFilenameGenerator { 
    /** Path that will store recorded videos. */ 
    public String recordPath = "recordedStreams/"; 
    /** Path that contains VOD streams. */ 
    public String playbackPath = "videoStreams/"; 
    /** Set if the path is absolute or relative */ 
    public boolean resolvesAbsolutePath = false; 

    public String generateFilename(IScope scope, String name, GenerationType type) { 
        // Generate filename without an extension. 
       return generateFilename(scope, name, null, type); 
    }

    public String generateFilename(IScope scope, String name, String extension, GenerationType type) { 
        String filename; 
        if (type == GenerationType.RECORD) 
            filename = recordPath + name; 
        else 
            filename = playbackPath + name; 

        if (extension != null) 
            // Add extension 
            filename += extension; 
  
        return filename; 
    } 

    public boolean resolvesToAbsolutePath() 
    { 
        return resolvesAbsolutePath; 
    } 
} 
//}

== カスタム・ジェネレータをアクティベートする

カスタム・ジェネレータは設定ファイルにてアクティベートする。
//emlist{
<bean id="streamFilenameGenerator" class="path.to.your.CustomFilenameGenerator" />
//}

== 設定でパスを変更する

パスを変更するたびにファイルをコンパイルし直すのは不便なので、上で定義したbeanに設定ファイル内でパスを設定できるようにする。

以下の３つのメソッドをクラスに追加する。
//emlist{
public void setRecordPath(String path) { 
    recordPath = path; 
} 
public void setPlaybackPath(String path) { 
    playbackPath = path; 
} 
public void setAbsolutePath(Boolean absolute) { 
    resolvesAbsolutePath = absolute; 
} 
//}
そうすりゃbeanの定義内でパスをセットできる。
//emlist{
<bean id="streamFilenameGenerator" class="path.to.your.CustomFilenameGenerator"> 
   <property name="recordPath" value="recordedStreams/" /> 
   <property name="playbackPath" value="videoStreams/" /> 
   <property name="absolutePath" value="false" /> 
</bean> 
<bean id="streamFilenameGenerator" class="path.to.your.CustomFilenameGenerator"> 
   <property name="recordPath" value="/path/to/recordedStreams/" /> 
   <property name="playbackPath" value="/path/to/videoStreams/" /> 
   <property name="absolutePath" value="true" /> 
</bean> 
//}
パスの定義を@<code>{{アプリのディレクトリ\}/WEB-INF/red5-web.properties}に書いて、
//emlist{
<bean id="streamFilenameGenerator" 
   class="path.to.your.CustomFilenameGenerator"> 
   <property name="recordPath" value="${recordPath}" /> 
   <property name="playbackPath" value="${playbackPath}" /> 
   <property name="absolutePath" value="${absolutePath}" /> 
</bean> 
//}
とするのもよし。
このとき、@<code>{red5-web.properties}には次のように書いておく。
//emlist{
recordPath=recordedStreams/ 
playbackPath=videoStreams/ 
absolutePath=false 
recordPath=/path/to/recordedStreams/ 
playbackPath=/path/to/videoStreams/ 
absolutePath=true
//}
