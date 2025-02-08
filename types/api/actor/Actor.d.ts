/// <reference path="../../index.d.ts"/>

/**
 * 活体实体，例如怪物或玩家。这是一个基础抽象类，代表所有具有移动特性的实体。
 * @hideconstructor
 */
declare class Actor extends CommandSender {
    toString(): "<Actor>" | string;

    /**
     * @brief 获取该实体的类型。
     *
     * 该方法返回实体的类型作为字符串，例如minecraft:pig。它用于获取实体的类型信息，以便可以为其提供相应的功能或属性。
     *
     * @return 实体的类型。
     */
    getType(): string;

    /**
     * @brief 获取实体的运行时ID。
     *
     * 运行时ID是用于标识单个实体的唯一标识符。它在服务器之间传递实体信息，并且可以在命令中引用特定的实体。
     *
     * @return 实体的运行时ID。
     */
    getRuntimeId(): number;

    /**
     * 获取实体当前的位置。
     *
     * @return 新的 Location 对象，包含该实体的位置信息。这个位置是相对世界坐标（WorldCoordinates）的一个副本，而不是原始的 Location 对象的引用。因此，修改返回值将不会影响到原 Entity 实例的当前位置。
     */
    // getLocation():Location;

    /**
     * 获取此角色的当前速度
     *
     * @return 当前移动的速度这个角色
     */
    // getVelocity(): number[];

    /**
     * 判断该实体是否在地面上（即块下）。返回真如果是这样的实体。
     *
     * @return true如果实体在地面上（即块下）。
     */
    isOnGround(): boolean;

    /**
     * 判断该角色是否处于水中
     *
     * @return 如果该角色在水中，返回true
     */
    isInWater(): boolean;

    /**
     * 判断该角色是否处于熔岩中
     *
     * @return 如果该角色在熔岩中，返回true
     */
    isInLava(): boolean;

    /**
     * 获取此角色所在的当前级别
     *
     * @return 此角色所处的当前级别
     */
    //getLevel():Level;

    /**
     * 获取此角色所在的当前维度
     *
     * @return 此角色所处的当前维度
     */
    //getDimension():Dimension;

    /**
     * 设置角色的旋转。
     * <p>
     * 注意，如果该角色受到AI影响，则可能会覆盖这个旋转。
     * </p>
     * @param yaw 围绕Y轴旋转的旋转（Y轴）
     * @param pitch 绕X轴旋转的旋转（X轴）
     */
    // setRotation(yaw: number, pitch: number): void;

    /**
     * 使该实体移动到指定的位置。位置由 Location 对象表示，它是一个相对世界坐标的副本，而不是原始的 Location 对象的引用。
     *
     * @param location 新的位置对象，用于使该实体移动到该位置。
     */
    //teleport(location: Location):void; //TODO Location

    /**
     * 使该实体移动到指定的另一个实体。目标实体必须是同一个服务器上的实体，并且可以被引用。如果目标实体不在当前服务器上，此方法将无法执行。
     *
     * @param target 目标实体对象。
     */
    //teleport(target: Actor):void; //TODO Actor

    /**
     * 获取该实体的唯一标识符（ID）。ID是用于标识单个实体的唯一标识符，通常是一个整数。它在服务器之间传递实体信息，并且可以在命令中引用特定的实体。
     *
     * @return 实体的唯一标识符（ID）。
     */
    getId(): number;

    /**
     * 判断该实体是否已经被标记为死亡或已删除。返回真如果是这样的实体，表示该实体已经死亡，并且无法进行任何操作。
     *
     * @return true如果实体已经被标记为死亡或已删除。
     */
    isDead(): boolean;

    /**
     * 获取实体的健康值。健康值是从 0 到最大值，其中 0 表示该实体已经死亡。这个方法用于获取和设置实体的健康值。如果健康值小于等于 0，则表示该实体已死亡，并且无法进行任何操作。
     *
     * @return 实体的健康值。
     */
    getHealth(): number;

    /**
     * 设置实体的健康值。健康值是从 0 到最大值，其中 0 表示该实体已经死亡。这个方法用于获取和设置实体的健康值。如果健康值小于等于 0，则表示该实体已死亡，并且无法进行任何操作。
     *
     * @param health 实体的健康值。
     */
    setHealth(health: number): void;

    /**
     * 获取实体的最大健康值。最大健康值是实体可以承受的最大生命值。这个方法用于获取和设置实体的最大健康值。如果最大健康值为零，则表示该实体无法承受任何生命值，并且无法进行任何操作。
     *
     * @return 实体的最大健康值。
     */
    getMaxHealth(): number;

    /**
     * 获取该实体的所有 scoreboard 标签。这些标签通常用于在服务器上显示和管理玩家的统计信息或属性。
     *
     * @return 实体的所有 scoreboard 标签列表。
     */
    getScoreboardTags(): string[];

    /**
     * 向该实体添加一个标签。如果标签已经存在，则返回 false，否则返回 true。标签通常用于在服务器上显示和管理玩家的统计信息或属性。
     *
     * @param tag 要添加的标签。
     * @return 如果标签成功添加则返回 true，否则返回 false。
     */
    addScoreboardTag(tag: string): boolean;

    /**
     * 从该实体中删除一个标签。如果标签不存在，则返回 false，否则返回 true。标签通常用于在服务器上显示和管理玩家的统计信息或属性。
     *
     * @param tag 要删除的标签。
     * @return 如果标签成功删除则返回 true，否则返回 false。
     */
    removeScoreboardTag(
        /** 标签 */
        tag: string
    ): boolean;

    /**
     * 判断该实体的名称标签是否当前可见。如果名称标签是当前可见，则返回 true，否则返回false。名称标签通常用于在服务器上显示和管理玩家的名称信息或属性。
     *
     * @return 名称标签是否当前可见。
     */
    isNameTagVisible(): boolean;

    /**
     * 设置该实体的名称标签为可见或不可见。如果名称标签是不可见，则返回 false，否则返回 true。名称标签通常用于在服务器上显示和管理玩家的名称信息或属性。
     *
     * @param visible 是否设置名称标签为可见。
     */
    setNameTagVisible(visible: string): void;

    /**
     * 判断该实体的名称标签是否当前不可见。如果名称标签是不可见，则返回 true，否则返回false。名称标签通常用于在服务器上显示和管理玩家的名称信息或属性。
     *
     * @return 名称标签是否当前不可见。
     */
    isNameTagAlwaysVisible(): boolean;

    /**
     * 设置该实体的名称标签为不可见或可见。如果名称标签是可见，则返回 false，否则返回true。名称标签通常用于在服务器上显示和管理玩家的名称信息或属性。
     *
     * @param invisible 是否设置名称标签为不可见。
     */
    setNameTagAlwaysVisible(invisible: string): boolean;

    /**
     * 获取角色的当前名称标签。
     *
     * @return 名称标签。
     */
    getNameTag(): string;

    /**
     * 设置角色的名称标签。
     *
     * @param name 新的名称标签。
     */
    setNameTag(name: string): void;

    /**
     * 获取角色的当前分数标签。
     *
     * @return 分数标签。
     */
    getScoreTag(): string;

    /**
     * 设置角色的分数标签。
     *
     * @param score 新的分数标签。
     */
    setScoreTag(score: string): void;
}
