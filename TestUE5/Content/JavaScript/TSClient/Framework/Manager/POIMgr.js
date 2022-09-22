"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const Logger_1 = require("Framework/Common/Logger");
/*
*  兴趣点管理类
*/
class POIMgr {
    constructor() { }
    static Instance;
    POIMap = new Map();
    /**
     * 获取单例
     */
    static GetInstance() {
        if (!this.Instance) {
            this.Instance = new POIMgr();
        }
        return this.Instance;
    }
    AddPOI(name, POI) {
        //if (!this.POIMap.has(name))
        {
            this.POIMap.set(name, POI);
        }
        Logger_1.Logger.log("[POIMgr add poi ] name: ", name, "size: ", this.POIMap.size);
    }
    RemovePOI(name) {
        if (this.POIMap.has(name)) {
            this.POIMap.delete(name);
        }
    }
    GetPOI(name) {
        if (this.POIMap.has(name)) {
            return this.POIMap.get(name);
        }
        return null;
    }
    ClearPOI() {
        this.POIMap.clear();
    }
}
exports.default = POIMgr;
//# sourceMappingURL=POIMgr.js.map