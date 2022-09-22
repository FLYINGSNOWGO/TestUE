"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.DownInfo = void 0;
const Delegates_1 = require("Framework/Common/Delegates");
const Logger_1 = require("Framework/Common/Logger");
const UE = require("ue");
class DownInfo {
    /**下载地址*/
    DownUrl;
    /**文件名称 */
    FileName;
    /**保存目录 */
    SaveRoot;
    /**下载的文件路径 */
    FileFullPath;
    /**远端MD5 */
    RemoteMd5;
    /**文件大小 */
    FileSize;
    /**打印信息 */
    PrintInfo() {
        Logger_1.Logger.warn("文件名称:" + this.FileName);
        Logger_1.Logger.warn("下载地址:" + this.DownUrl);
        Logger_1.Logger.warn("保存目录:" + this.SaveRoot);
        Logger_1.Logger.warn("文件路径:" + this.FileFullPath);
        Logger_1.Logger.warn("远端MD5:" + this.RemoteMd5);
        Logger_1.Logger.warn("文件大小:" + this.FileSize);
    }
}
exports.DownInfo = DownInfo;
class Dowunloader {
    /**下载信息 */
    DownInfo;
    /**下载实例GUID */
    Guid;
    /**收到文件大小回调 调用 reallyDown 才会真正的下载 */
    DownloadFileSizeDelegate = new Delegates_1.TSDelegate();
    /**下载完成回调 */
    DownloadFileCompleteDelegate = new Delegates_1.TSDelegate();
    /**下载错误回调 */
    DownloadFileErrorDelegate = new Delegates_1.TSDelegate();
    /**下载进度回调 */
    DownloadProgressDelegate = new Delegates_1.TSDelegate();
    /**下载失败后重试下载回调 */
    DownloadFileWillRetryDelegate = new Delegates_1.TSDelegate();
    static Create(DownInfo) {
        let obj = new Dowunloader();
        obj.Init(DownInfo);
        return obj;
    }
    Init(DownInfo) {
        this.DownInfo = DownInfo;
    }
    /**
     * 开始下载，在DownloadFileSizeDelegate 回调中要调用ReallyDown
     */
    StartDown() {
        this.CancleDown();
        Logger_1.Logger.warn("APPDowunloader");
        this.DownInfo.PrintInfo();
        let fileEntry = new UE.TSDownloadFileEntry();
        fileEntry.FileFullPath = this.DownInfo.FileFullPath;
        ;
        fileEntry.FileName = this.DownInfo.FileName;
        ;
        fileEntry.RemoteMd5 = this.DownInfo.RemoteMd5;
        fileEntry.RelativeUrl = this.DownInfo.DownUrl;
        fileEntry.FileSize = this.DownInfo.FileSize;
        fileEntry.DownloadFileSizeDelegate.Bind(this.OnDownloadFileSizeDelegate.bind(this));
        fileEntry.DownloadFileCompleteDelegate.Bind(this.OnDownloadFileCompleteDelegate.bind(this));
        fileEntry.DownloadFileErrorDelegate.Bind(this.OnDownloadFileErrorDelegate.bind(this));
        fileEntry.DownloadProgressDelegate.Bind(this.OnDownloadProgressDelegate.bind(this));
        fileEntry.DownloadFileWillRetryDelegate.Bind(this.OnDownloadFileWillRetryDelegate.bind(this));
        fileEntry.DownloadFileSizeDelegate.Bind(this.OnDownloadFileWillRetryDelegate.bind(this));
        this.Guid = UE.TSDownloadManager.GetTSDownloadManagerInstance().DownLoad(fileEntry);
    }
    CancleDown() {
        if (this.Guid) {
            UE.TSDownloadManager.GetTSDownloadManagerInstance().CancleDown(this.Guid);
        }
        this.Guid = null;
    }
    OnDownloadFileSizeDelegate(FileName, FileSize) {
        this.DownloadFileSizeDelegate.Broadcast(this, FileSize);
    }
    OnDownloadFileCompleteDelegate(A) {
        this.DownloadFileCompleteDelegate.Broadcast(this, A);
    }
    OnDownloadFileErrorDelegate(A, B) {
        this.DownloadFileErrorDelegate.Broadcast(this, A, B);
    }
    OnDownloadProgressDelegate(A, B, C) {
        this.DownloadProgressDelegate.Broadcast(this, A, B, C);
    }
    OnDownloadFileWillRetryDelegate(A) {
        this.DownloadFileWillRetryDelegate.Broadcast(this, A);
    }
}
exports.default = Dowunloader;
//# sourceMappingURL=Downloader.js.map