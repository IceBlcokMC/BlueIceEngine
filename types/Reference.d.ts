//----------------------------------------------------------------
// C++ Reference https://github.com/puerts/puerts_node/blob/master/lib/index.d.ts
//----------------------------------------------------------------

declare interface $Ref<T> {
  __doNoAccess: T;
}

declare interface $InRef<T> {
  __doNoAccess: T;
}

declare type $Nullable<T> = T | null;

declare type cstring = string | ArrayBuffer;

declare function $ref<T>(x?: T): $Ref<T>; // [x]
declare function $unref<T>(x: $Ref<T> | $InRef<T>): T; // x[0]
