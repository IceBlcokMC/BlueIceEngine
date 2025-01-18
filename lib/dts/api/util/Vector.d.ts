/// <reference path="../../index.d.ts"/>

// Vector<float>
declare class Vector {
    constructor(x: number, y: number, z: number);

    x: number; // float
    y: number; // float
    z: number; // float

    toString(): string;

    getX(): number; // float
    getY(): number; // float
    getZ(): number; // float

    setX(value: number): number; // float
    setY(value: number): number; // float
    setZ(value: number): number; // float

    length(): number; // float
    lengthSquared(): number; // float

    distance(other: Vector): number; // float
    distanceSquared(other: Vector): number; // float
}
