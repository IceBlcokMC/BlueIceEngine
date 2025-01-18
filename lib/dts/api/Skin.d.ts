/// <reference path="../index.d.ts"/>

declare class Skin {
    toString(): "<Skin>" | string;

    getSkinId(): string;

    getSkinData(): SkinImageData;

    getCapeId(): string;

    getCapeData(): SkinImageData;
}

// endstone::Skin::ImageData
declare class SkinImageData {
    constructor(height: number, width: number, data: string);

    height: number;
    width: number;
    data: string;
}
