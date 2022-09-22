"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.EPlatform = exports.PlatformHelper = void 0;
const UE = require("ue");
/**设备平台  */
var EPlatform;
(function (EPlatform) {
    EPlatform[EPlatform["Android"] = 0] = "Android";
    EPlatform[EPlatform["Ios"] = 1] = "Ios";
    EPlatform[EPlatform["Windows"] = 2] = "Windows";
    EPlatform[EPlatform["Unknown"] = 3] = "Unknown";
})(EPlatform || (EPlatform = {}));
exports.EPlatform = EPlatform;
class PlatformHelper {
    static CurPlatForm = null;
    static GetPlatform() {
        //if (!this.CurPlatForm)
        {
            let pN = UE.GameplayStatics.GetPlatformName();
            console.log("PlatformHelper.GetPlatform UE.GameplayStatics.GetPlatformName = " + pN);
            if (pN == "Android") {
                this.CurPlatForm = EPlatform.Android;
            }
            else if (pN == "IOS") {
                this.CurPlatForm = EPlatform.Ios;
            }
            else if (pN == "Windows") {
                this.CurPlatForm = EPlatform.Windows;
            }
            else {
                this.CurPlatForm = EPlatform.Unknown;
            }
        }
        console.log("PlatformHelper.GetPlatform = " + this.CurPlatForm);
        return this.CurPlatForm;
    }
}
exports.PlatformHelper = PlatformHelper;
//# sourceMappingURL=PlatformHelper.js.map