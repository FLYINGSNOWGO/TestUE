"use strict";
// export class DataConfig
// {
//     static DataCache = new Map<string, BaseConfConf>();
//     static GetConfigByName(name: string)
//     {
//         if (!name || name == "")
//         {
//             name = "default";
//             if (!this.DataCache.has(name))
//             {
//                 this.DataCache.set(name, new BaseConfConf())
//             }
//         }
//         if (!this.DataCache.has(name))
//         {
//             if (name == "ginger")
//             {
//                 this.DataCache.set(name, new PlayerConf_Ginger());
//             }
//             else if (name == "Ginger_Lite")
//             {
//                 this.DataCache.set(name, new PlayerConf_Ginger())
//             }
//             else if (name == "Cloundia")
//             {
//                 this.DataCache.set(name, new PlayerConf_Ginger)
//             }
//         }
Object.defineProperty(exports, "__esModule", { value: true });
exports.AIDataConfig = exports.DataConfig = void 0;
//         let data = this.DataCache.get(name);
//         if (!data)
//         {
//             console.error("获取配置数据错误 " + name);
//         }
//         return data;
//     }
//     /**机器人的蓝图配置***/
//     static RobotBPConf = new Map
//         ([
//             [1001, { configID: "Robot_Ginger", RoleBP: "Blueprint'/Game/DigitalTwins/ginger.ginger_C'" }],
//             [1002, { configID: "Robot_Ginger", RoleBP: "Blueprint'/Game/DigitalTwins/ginger.ginger_C'" }],
//         ]);
//     static GetRobotBPConf(nID: number): any
//     {
//         return this.RobotBPConf.get(nID)
//     }
// }
/**上海展厅配置*Begin*/
class DataConfig {
    //CCU to UE4
    static SH_ZhanTing_1 = new Map([
        ["Rotation", { X: -0, Y: 0, Z: -0.678801, W: 0.734323 }],
        ["Translation", { X: -2892, Y: 2036, Z: 4 }],
        ["Scale", { X: 5, Y: 5, Z: 1 }],
    ]);
    //UE to CCU
    static SH_ZhanTing_2 = new Map([
        ["Rotation", { X: 0, Y: 0, Z: 0.678801, W: 0.734323 }],
        ["Translation", { X: 451.325479, Y: 544.668442, Z: -4 }],
        ["Scale", { X: 0.2, Y: 0.2, Z: 1 }],
    ]);
}
exports.DataConfig = DataConfig;
/**上海展厅配置*End*/
/*****AI使用的配置数据*****/
class AIDataConfig {
    //是否使用本地的测试角色
    static bUseLocalTest = true;
    /**行为树节点的配置
     * @key 节点的名字
     * @ TaskID 任务的ID
     * @ FixTime  任务无响应时的修复时长
    */
    static AITaskNodeData = new Map([
        ["Task_HA", { TaskID: 1, FixTime: 10 }],
        ["Task_Skill", { TaskID: 2, FixTime: 10 }],
        ["Task_Join", { TaskID: 3, FixTime: 10 }],
    ]);
    /**黑板CDKey
     * 标注的对应的task的名字数组
    */
    static TaskNodeKeys = {
        Task_HA: "Task_HA",
        Task_Skill: "Task_Skill",
        Task_Join: "Task_Join", //灵动模式
        // Task_
    };
}
exports.AIDataConfig = AIDataConfig;
//# sourceMappingURL=DataConfig.js.map