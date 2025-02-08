/// <reference path="../../index.d.ts"/>

/**
 * 表示一个IP套接字地址（主机名和端口号）。
 */
declare class SocketAddress {
    constructor(hostname: string, port: number);

    toString(): "<SocketAddress>" | string;

    /**
     * 获取此套接字地址的主机名。
     *
     * @return 该套接字地址的主机名
     */
    getHostname(): string;

    /**
     * 获取此套接字地址的端口号。
     *
     * @return 该套接字地址的端口号
     */
    getPort(): number;
}
