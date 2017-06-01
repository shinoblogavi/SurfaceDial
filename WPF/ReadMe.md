# Surface Dial WPF Implement
WPF にSurface Dial を対応させるにはいくつかの手順が必要である。基本的な方法は、Win32 アプリケーションのインターフェースを使ってCOMオブジェクトを作成し、そこからRadialContoller のオブジェクトを使うという形。

## Windows Runtime の参照

WPFでWindows Runtime を扱うために、まずはWindows Runtime の参照が必要となる。

```
<Reference Include="Windows.winmd">
    <HintPath>$(MSBuildProgramFiles32)\Windows Kits\10\UnionMetadata\Windows.winmd</HintPath>
</Reference>
```

## Comオブジェクト取得のためのインターフェース作成
続けて、Radial Contoller と RadialControllerConfigration インスタンスを取得するための、
Comオブジェクトを取得するためのインターフェースを準備する
ここはMicrosoft のサンプルが参考になる。
<https://github.com/Microsoft/Windows-classic-samples/tree/master/Samples/RadialController>

このインターフェースは再利用可能なので、1度作っておけばあとは楽になります。

```
IDesktopRadialController.cs
[Guid("1b0535c9-57ad-45c1-9d79-ad5c34360513")]
[InterfaceType(ComInterfaceType.InterfaceIsIInspectable)]
public interface IDesktopRadialController
{
    RadialController CreateForWindow(IntPtr hWnd, [In] ref Guid iid);
}
```
```
[Guid("787cdaac-3186-476d-87e4-b9374a7b9970")]
[InterfaceType(ComInterfaceType.InterfaceIsIInspectable)]
public interface IDesktopRadialControllerConfiguration
{
    RadialControllerConfiguration GetForWindow(IntPtr hWnd, [In] ref Guid iid);
}
```

## アプリからのインターフェースを経由したWindows Rutimeの利用

利用時には初めに、インターフェース経由でインスタンスを取得し、APIを利用。

DesktopRadialController.cs
```
public static class DesktopRadialController
{
    public static RadialController Create(IntPtr hWnd)
    {
        var controller =
            (IDesktopRadialController)WindowsRuntimeMarshal.GetActivationFactory(typeof(RadialController));
        var iid = typeof(RadialController).GetInterface("IRadialController").GUID;
        return controller.CreateForWindow(hWnd, ref iid);
    }
}
```
```
public static class DesktopRadialControllerConfiguration
{
    public static RadialControllerConfiguration Create(IntPtr hWnd)
    {
        var configration = (IDesktopRadialControllerConfiguration)WindowsRuntimeMarshal.
            GetActivationFactory(typeof(RadialControllerConfiguration));
        var iid = typeof(RadialControllerConfiguration).GetInterface("IRadialControllerConfiguration").GUID;
        return configration.GetForWindow(hWnd, ref iid);
    }
}
```

## Dial の機能の利用

実際にDaial の機能を使う方法は、UWPでの実装と大きくは変わらない。
初めに標準メニューに対するカスタマイズをする

ウィンドウ ハンドルから RadialController と RadialControllerConfiguration のインスタンス取得
```
var controller = DesktopRadialController.Create(source.Handle);
var configuration = DesktopRadialControllerConfiguration.Create(source.Handle);

```

既定メニューのうちアプリで使いたいものを選んで設定
```
configuration.SetDefaultMenuItems(new[] { RadialControllerSystemMenuItemKind.Volume, });

// アプリ独自メニューを追加
controller.Menu.Items.Add(
    RadialControllerMenuItem.CreateFromKnownIcon(
        "Tab", 
        RadialControllerMenuKnownIcon.Scroll
    )
);
```

メニュー項目をカスタマイズする場合のアイコンの処理
* リソースに置かれた画像ファイルからアイコンを取得する
* デフォルトのアイコンを使う場合: CreateFromKnownIcon()
* オリジナルの画像ファイルを使う場合: CreateFromIcon() , ソースは IRandomAccessStreamReference
* 以下の実装には IAsyncOeration のために System.Runtime.WindowsRuntime.dllの参照が必要

```
async void InitializeController()
{
   var controller = this.CreateRadialController();// 中略
   const string iconUri = "pack://application:,,,/SampleAssembly;Component/assets/Item0.png";
   var menuItem = await CreateMenuItem("Sample", new Uri(iconUri, UriKind.Absolute));

   controller.Menu.Items.Add(menuItem);
}
```


```
static async Task<RadialControllerMenuItem> CreateMenuItem(string displayText, Uri iconUri)
{
    // Get Stream from IconURL
    var resourceInfo = Application.GetResourceStream(iconUri);
    if (resourceInfo == null) throw new ArgumentException("Resource not found.", nameof(iconUri));

    using (var stream = resourceInfo.Stream)
    {
        // Get Byte Allay from Strream
        var array = new byte[stream.Length];
        stream.Read(array, 0, array.Length);

        using (var randomAccessStream = new InMemoryRandomAccessStream())
        {
            // Wrote byte allay to UWP Stream
            await randomAccessStream.WriteAsync(array.AsBuffer());
            return RadialControllerMenuItem.CreateFromIcon(
                displayText, 
                RandomAccessStreamReference.CreateFromStream(randomAccessStream));
        }
    }
}
```
## 続けてイベントの取得とその実装。

Dial の回転イベントを定義 (args.RotationDeltaInDegrees で回転角度を取ったりする)
```
controller.RotationChanged += (sender, args) => Action(args.RotationDeltaInDegrees);
```