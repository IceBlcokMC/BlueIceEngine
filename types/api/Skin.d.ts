/// <reference path="../index.d.ts"/>

/**
 * 玩家皮肤。
 * @hideconstructor
 */
declare class Skin {
    toString(): "<Skin>" | string;

    /**
     * 获取皮肤ID。
     *
     * @return 皮肤ID。
     */
    getSkinId(): string;

    /**
     * 获取皮肤数据。
     *
     * @return 皮肤数据。
     */
    getSkinData(): SkinImageData;

    /**
     * 获取披风ID。
     *
     * @return 披风ID。
     */
    getCapeId(): string;

    /**
     * 获取披风数据。
     *
     * @return 披风数据。
     */
    getCapeData(): SkinImageData;
}

// endstone::Skin::ImageData
declare class SkinImageData {
    constructor(height: number, width: number, data: string);

    height: number;
    width: number;
    data: string;
}
