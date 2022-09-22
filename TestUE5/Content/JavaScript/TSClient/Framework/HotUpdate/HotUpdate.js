"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
/*
export default class HotUpdate
{

    HotUpdateSubsystem: UE.GameHotUpdateSubsystem;
    UIWidget: UE.HotUpdateMain_C;

    CanvasPanelProgress: UE.Widget;
    CanvasPanel_UpdateInfo: UE.Widget;
    CanvasPanel_Error: UE.Widget;

    UpdateaFinishCallback: Function;
    TextBlock_Progress: UE.TextBlock;
    TextBlock_dowoSize: UE.TextBlock;
    TextBlock_Error: UE.TextBlock;
    Init(ins: UE.GameInstance)
    {

        this.UpdateaFinishCallback = null;

        let world = ins.GetWorld();

        let uibp = UE.Class.Load("WidgetBlueprint'/Game/HotUpdate/HotUpdateMain.HotUpdateMain_C'");
        let widget = UE.GameHelper.CreateWidget(world, uibp) as UE.HotUpdateMain_C;
        widget.AddToViewport(0);


        this.UIWidget = widget;

        this.HotUpdateSubsystem = UE.SubsystemBlueprintLibrary.GetGameInstanceSubsystem(world, UE.GameHotUpdateSubsystem.StaticClass()) as UE.GameHotUpdateSubsystem;

        this.CanvasPanelProgress = UE.GameHelper.GetWidgetFromName(this.UIWidget, "CanvasPanelProgress")
        this.CanvasPanel_UpdateInfo = UE.GameHelper.GetWidgetFromName(this.UIWidget, "CanvasPanel_UpdateInfo");
        this.CanvasPanel_Error = UE.GameHelper.GetWidgetFromName(this.UIWidget, "CanvasPanel_Error");

        this.TextBlock_Progress = UE.GameHelper.GetWidgetFromName(this.UIWidget, "TextBlock_Progress") as UE.TextBlock;
        this.TextBlock_dowoSize = UE.GameHelper.GetWidgetFromName(this.UIWidget, "TextBlock_dowoSize") as UE.TextBlock;
        this.TextBlock_Error = UE.GameHelper.GetWidgetFromName(this.UIWidget, "TextBlock_Error") as UE.TextBlock;

        this.UIWidget.ProgressBar_progress.SetVisibility(UE.ESlateVisibility.Hidden);
        this.CanvasPanel_UpdateInfo.SetVisibility(UE.ESlateVisibility.Hidden);
        this.UIWidget.Image_progress.SetVisibility(UE.ESlateVisibility.Hidden);
        this.CanvasPanel_Error.SetVisibility(UE.ESlateVisibility.Hidden);


        this.Clean();


        if (!this.HotUpdateSubsystem)
        {
            console.log("ERROR 下载器初始化失败")
            this.UpdateaFinish(false);
            return;
        }

        this.UIWidget.Button_OK.OnClicked.Add(this.OnUpdateClick.bind(this));


        this.HotUpdateSubsystem.OnUpdateStateNotify.Add(this.OnUpdateStateNotify.bind(this))
        this.HotUpdateSubsystem.OnDownloadUpdate.Add(this.OnDownloadUpdate.bind(this));
        this.HotUpdateSubsystem.OnGameHotUpdateFinished.Add(this.OnHotUpdateFinished.bind(this));
        this.HotUpdateSubsystem.OnMountUpdate.Add(this.OnMountUpdate.bind(this));
        this.UIWidget.Button_EnterGame.OnClicked.Add(this.EnterGame.bind(this));

    }

    CheckUpdate(module: string = null)
    {
        if (module)
        {
            this.HotUpdateSubsystem.StartUpMap(module);
        }
        else
        {
            this.HotUpdateSubsystem.StartUp();
        }

    }



    Clean()
    {
        this.HotUpdateSubsystem.OnUpdateStateNotify.Clear();
        this.HotUpdateSubsystem.OnDownloadUpdate.Clear();
        this.HotUpdateSubsystem.OnGameHotUpdateFinished.Clear();
        this.HotUpdateSubsystem.OnMountUpdate.Clear();
    }

    OnUpdateClick()
    {
        this.HotUpdateSubsystem.StartDownPak();
        this.CanvasPanel_UpdateInfo.SetVisibility(UE.ESlateVisibility.Hidden);
        this.UIWidget.ProgressBar_progress.SetVisibility(UE.ESlateVisibility.Visible);
        this.UIWidget.ProgressBar_progress.SetPercent(0);

    }

    OnDownloadUpdate(dp: UE.DownloadProgress)
    {
        console.log("[HotUpdateSystem] Desc:" + "CurrentDownloadSize:" + dp.CurrentDownloadSize + "DownloadSpeed:" + dp.DownloadSpeed + "TotalDownloadSize:" + dp.TotalDownloadSize + "");
        let per = dp.DownloadPer;
        this.UIWidget.ProgressBar_progress.SetPercent(per);
        let str = "资源更新中..." + (per * 100).toFixed(2) + "%    " + "速度：" + dp.DownloadSpeed + "      " + this.ConvertIntToSize(dp.CurrentDownloadSize) + "/" + this.ConvertIntToSize(dp.TotalDownloadSize)
        this.TextBlock_Progress.SetText(str);

    }

    OnHotUpdateFinished()
    {
        console.log("[HotUpdateSystem] OnHotUpdateFinished");
        this.TextBlock_Progress.SetText("更新完成，即将进入游戏");
        this.UpdateaFinish(true);
    }

    OnMountUpdate(PakName: string, Progress: number)
    {
        console.log("[HotUpdateSystem] OnMountUpdate PakName " + PakName + " Progress:" + Progress);
    }

    OnUpdateStateNotify(State: UE.EGameHotUpdateState, msg: string)
    {
        console.log("[HotUpdateSystem] OnUpdateStateNotify:" + State + " msg:" + msg);
        switch (State)
        {
            case UE.EGameHotUpdateState.END_GETVERSION:
                {
                    this.OnGetVersionInfo(msg);
                    break;
                }
            case UE.EGameHotUpdateState.BEGIN_DOWNLOAD:
                {
                    console.warn("[HotUpdateSystem] OnUpdateStateNotify:" + State + " msg:" + msg);
                    break;
                }
            case UE.EGameHotUpdateState.BEGIN_MOUNT:
                {
                    this.TextBlock_Progress.SetText("资源下载完成，正在加载资源");
                    break;
                }
            case UE.EGameHotUpdateState.ERROR:
                {
                    console.error("[HotUpdateSystem] 更新失败：" + msg);
                    this.CanvasPanel_Error.SetVisibility(UE.ESlateVisibility.Visible);
                    this.TextBlock_Error.SetText(msg);
                    break;
                }
        }
    }

    OnGetVersionInfo(versionJson: string)
    {
        let totalSize = BigInt(this.HotUpdateSubsystem.TotalDownSize);
        console.log("[HotUpdateSystem] 更新大小 " + totalSize)

        if (totalSize > 0)
        {
            this.CanvasPanel_UpdateInfo.SetVisibility(UE.ESlateVisibility.Visible);
            let text = this.TextBlock_dowoSize.GetText();
            let str = this.ConvertIntToSize(totalSize);
            text = text.replace("%s", str)
            this.TextBlock_dowoSize.SetText(text);
        }
        else
        {
            this.OnUpdateClick();
        }

    }


    UpdateaFinish(bSuccess: boolean)
    {
        if (this.UpdateaFinishCallback)
        {
            this.UpdateaFinishCallback.call(bSuccess);
        }
    }


    ConvertIntToSize(size: bigint)
    {
        if (size < 1024n)
        {
            return size + "B";
        }

        size /= 1024n;

        if (size < 1024n)
        {
            return size + "KB";
        }

        size /= 1024n;

        return size + "MB";
    }


    EnterGame()
    {
        this.UpdateaFinish(false);
    }
}

*/ 
//# sourceMappingURL=HotUpdate.js.map