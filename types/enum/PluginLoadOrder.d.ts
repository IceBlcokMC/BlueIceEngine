/// <reference path="../index.d.ts"/>

declare namespace Enums {
    /**
     * 表示插件应按初始化和启用的顺序排列。
     */
    enum PluginLoadOrder {
        /**
         * 指示插件将在启动时加载
         */
        Startup = 0,
        /**
         * 表示该插件将与第一个/默认世界创建后加载
         */
        PostWorld = 1,
    }
}
