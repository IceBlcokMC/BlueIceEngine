/// <reference path="../../index.d.ts"/>

declare class SocketAddress {
    constructor(hostname: string, port: number);

    toString(): "<SocketAddress>" | string;

    getHostname(): string;

    getPort(): number;
}
