/// <reference path="../index.d.ts"/>

/**
 * 日志类
 * @hideconstructor
 */
declare class Logger {
    toString(): "<Logger>";

    /**
     * 在指定的日志级别下记录消息。
     *
     * @param level 消息应该被记录的日志级别的
     * @param message 要记录的消息
     */
    log(level: Enums.LoggerLevel, ...message: any[]): void;
    /** 输出信息日志 */
    info(...message: any[]): void;
    /** 输出警告日志 */
    warning(...message: any[]): void;
    /** 输出错误日志 */
    error(...message: any[]): void;
    /** 输出调试日志 */
    debug(...message: any[]): void;
    /** 输出跟踪日志 */
    trace(...message: any[]): void;
    /** 输出致命错误日志 */
    critical(...message: any[]): void;

    /**
     * 设置日志级别此日志实例。
     *
     * @param level 指定的日志级别
     */
    setLevel(level: Enums.LoggerLevel): void;

    /**
     * 检查 Logger 实例是否对于给定的日志级别启用。
     *
     * @param level 日志级别的检查
     * @return 如果日志是针对 Level 启用，返回 true，否则为 false。
     */
    isEnabledFor(level: Enums.LoggerLevel): boolean;

    /**
     * 获取此 Logger 实例的名称。
     *
     * @return 此 Logger 实例的名称。
     */
    getName(): string | undefined;
}
