"use strict";
/*
*  UI逻辑基类 可以作为一个功能逻辑模块
*/
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const AssetMgr_1 = require("Framework/Manager/AssetMgr");
const Logger_1 = require("Framework/Common/Logger");
class BaseFeature {
    Name;
    UIFile;
    UserWidget;
    Zorder;
    GameScene;
    /**配置定义 */
    Conf;
    constructor(conf, uiData) {
        this.Zorder = conf.Zorder ? conf.Zorder : 0;
        this.Name = conf.Name;
        this.UIFile = conf.UIPath;
        this.Conf = conf;
    }
    /**init */
    Init(Scene) {
        this.GameScene = Scene;
        if (this.UIFile) {
            let widget = AssetMgr_1.default.GetInstance().LoadWidget(this.UIFile);
            if (widget) {
                widget.AddToViewport(this.Zorder);
                this.UserWidget = widget;
            }
            else {
                Logger_1.Logger.error("创建UI失败 " + this.UIFile);
            }
        }
    }
    /**获取UI控件 */
    GetWidgetByName(name) {
        if (this.UserWidget) {
            let widget = UE.TSHelper.GetWidgetFromName(this.UserWidget, name);
            return widget;
        }
    }
    /**从场景移除 */
    RemoveFromScene() {
        this.OnClean();
        if (this.UserWidget) {
            this.UserWidget.RemoveFromParent();
            this.UserWidget = null;
        }
        if (this.GameScene) {
            this.GameScene.RemoveFeatureFromArr(this);
        }
        this.Name = null;
        this.UIFile = null;
        this.GameScene = null;
    }
    /************************************************************逻辑接口**************************************************/
    /**收到初始化消息 如果有UI文件，加了UI文件初始化完成后调用到此接口
     *
    */
    OnInit() {
        Logger_1.Logger.warn("BaseUI:OnInit " + this.Name, " UIPath:" + this.UIFile);
    }
    /**调用RemoveFromScene 后会收到此清理消息 清理自己类的数据 父类数据由父类自行清理 */
    OnClean() {
        Logger_1.Logger.warn("BaseUI:OnClean " + this.Name, " UIPath:" + this.UIFile);
    }
}
exports.default = BaseFeature;
//# sourceMappingURL=BaseFeature.js.map