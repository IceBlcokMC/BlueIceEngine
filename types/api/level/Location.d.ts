/// <reference path="../../index.d.ts"/>

/**
 * 该位置的三维位置表示，包含在维度中的一级。
 */
declare class Location extends Position {
    constructor(
        dim: Dimension,
        x: number,
        y: number,
        z: number,
        pitch: number,
        yaw: number
    );

    toString(): "<Location>" | string;

    /**
     * 获取这个位置的坡度，以度为单位测量。坡度是水平方向、负y方向、向上方向或正y方向之间的角度。
     *
     * @return 块的坡度
     */
    getPitch(): number;

    /**
     * 设置这个位置的坡度，以度为单位测量。坡度是水平方向、负y方向、向上方向或正y方向之间的角度。
     * <ul>
     * <li>一个坡度为0表示面向水平方向。</li>
     * <li>一个坡度为90表示向下方向（负Y方向）。</li>
     * <li>一个坡度为-90表示向上方向（正Y方向）。</li>
     * </ul>
     * 增加坡度值相当于向下看。
     *
     * @param pitch 新坡度的坡度
     */
    setPitch(pitch: number): void;

    /**
     * 获取这个位置的 yaw，以度为单位测量。yaw是顺时针方向或逆时针方向之间的角度。
     *
     * @return 旋转的yaw
     */
    getYaw(): number;

    /**
     * 设置这个位置的 yaw，以度为单位测量。yaw是顺时针方向或逆时针方向之间的角度。
     * <ul>
     * <li>一个 yaw为0或360表示正Z方向。</li>
     * <li>一个 yaw为180表示负Z方向。</li>
     * <li>一个 yaw为90表示负X方向。</li>
     * <li>一个 yaw为270表示正X方向。</li>
     * </ul>
     * 增加yaw值相当于向右-facing，增加下 respective轴的缩放，减少上 respective轴的缩放
     *
     * @param yaw 新旋转的yaw
     */
    setYaw(yaw: number): void;
}
