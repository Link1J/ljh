
;          Copyright Jared Irwin 2024-2026
; Distributed under the Boost Software License, Version 1.0.
;    (See accompanying file LICENSE_1_0.txt or copy at
;          https://www.boost.org/LICENSE_1_0.txt)

#include "ksarm64.h"

    AREA |.text$lj$21|, CODE, READONLY, ALIGN=5

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Safe Addition Functions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    EXPORT A64NAME(_ljh_asm_add_i8) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_add_i8),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY A64NAME(_ljh_asm_add_i8),|.text$$lj$$21|
        sxtb    w3, w1
        add     w3, w3, w2, sxtb
        cmp     w3, w3, sxtb
        strb    w3, [x0]
        cset    w0, ne
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_add_u8) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_add_u8),2,0,|.text$$lj$$21|,5
    LEAF_ENTRY A64NAME(_ljh_asm_add_u8),|.text$$lj$$21|
        and     w3, w1, #0xff
        add     w4, w3, w2, uxtb
        lsr     w3, w4, #8
        strb    w4, [x0]
        mov     w0, w3
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_add_i16) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_add_i16),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY_COMDAT A64NAME(_ljh_asm_add_i16),|.text$$lj$$21|
        sxth    w3, w1
        add     w3, w3, w2, sxth
        cmp     w3, w3, sxth
        strh    w3, [x0]
        cset    w0, ne
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_add_u16) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_add_u16),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY_COMDAT A64NAME(_ljh_asm_add_u16),|.text$$lj$$21|
        and     w3, w1, #0xffff
        add     w4, w3, w2, uxth
        lsr     w3, w4, #16
        strh    w4, [x0]
        mov     w0, w3
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_add_i32) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_add_i32),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY_COMDAT A64NAME(_ljh_asm_add_i32),|.text$$lj$$21|
        adds    w3, w1, w2
        str     w3, [x0]
        cset    w0, vs
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_add_u32) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_add_u32),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY_COMDAT A64NAME(_ljh_asm_add_u32),|.text$$lj$$21|
        adds    w3, w1, w2
        str     w3, [x0]
        cset    w0, hs
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_add_i64) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_add_i64),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY_COMDAT A64NAME(_ljh_asm_add_i64),|.text$$lj$$21|
        adds    x3, x1, x2
        str     x3, [x0]
        cset    w0, vs
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_add_u64) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_add_u64),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY_COMDAT A64NAME(_ljh_asm_add_u64),|.text$$lj$$21|
        adds    x3, x1, x2
        str     x3, [x0]
        cset    w0, hs
        ret
    LEAF_END

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Safe Subtract Functions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    EXPORT A64NAME(_ljh_asm_sub_i8) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_sub_i8),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY A64NAME(_ljh_asm_sub_i8),|.text$$lj$$21|
        sxtb    w3, w1
        sub     w3, w3, w2, sxtb
        cmp     w3, w3, sxtb
        strb    w3, [x0]
        cset    w0, ne
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_sub_u8) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_sub_u8),2,0,|.text$$lj$$21|,5
    LEAF_ENTRY A64NAME(_ljh_asm_sub_u8),|.text$$lj$$21|
        and     w3, w1, #0xff
        sub     w4, w3, w2, uxtb
        cmp     w3, w4, uxtb
        strb    w4, [x0]
        cset    w0, cc
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_sub_i16) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_sub_i16),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY_COMDAT A64NAME(_ljh_asm_sub_i16),|.text$$lj$$21|
        sxth    w3, w1
        sub     w3, w3, w2, sxth
        cmp     w3, w3, sxth
        strh    w3, [x0]
        cset    w0, ne
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_sub_u16) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_sub_u16),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY_COMDAT A64NAME(_ljh_asm_sub_u16),|.text$$lj$$21|
        and     w3, w1, #0xffff
        sub     w4, w3, w2, uxth
        cmp     w3, w4, uxth
        strh    w4, [x0]
        cset    w0, cc
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_sub_i32) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_sub_i32),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY_COMDAT A64NAME(_ljh_asm_sub_i32),|.text$$lj$$21|
        subs    w3, w1, w2
        str     w3, [x0]
        cset    w0, vs
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_sub_u32) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_sub_u32),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY_COMDAT A64NAME(_ljh_asm_sub_u32),|.text$$lj$$21|
        subs    w3, w1, w2
        str     w3, [x0]
        cset    w0, lo
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_sub_i64) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_sub_i64),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY_COMDAT A64NAME(_ljh_asm_sub_i64),|.text$$lj$$21|
        subs    x3, x1, x2
        str     x3, [x0]
        cset    w0, vs
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_sub_u64) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_sub_u64),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY_COMDAT A64NAME(_ljh_asm_sub_u64),|.text$$lj$$21|
        subs    x3, x1, x2
        str     x3, [x0]
        cset    w0, lo
        ret
    LEAF_END

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Safe Multiple Functions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    EXPORT A64NAME(_ljh_asm_mul_i8) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_mul_i8),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY A64NAME(_ljh_asm_mul_i8),|.text$$lj$$21|
        sxtb    w3, w2
        sxtb    w4, w1
        mul     w3, w4, w3
        cmp     w3, w3, sxtb
        strb    w3, [x0]
        cset    w0, ne
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_mul_u8) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_mul_u8),2,0,|.text$$lj$$21|,5
    LEAF_ENTRY A64NAME(_ljh_asm_mul_u8),|.text$$lj$$21|
        and     w3, w2, #0xff
        and     w4, w1, #0xff
        mul     w3, w4, w3
        strb    w3, [x0]
        cmp     wzr, w3, lsr 8
        cset    w0, ne
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_mul_i16) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_mul_i16),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY_COMDAT A64NAME(_ljh_asm_mul_i16),|.text$$lj$$21|
        sxth    w3, w2
        sxth    w4, w1
        mul     w3, w4, w3
        cmp     w3, w3, sxth
        strh    w3, [x0]
        cset    w0, ne
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_mul_u16) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_mul_u16),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY_COMDAT A64NAME(_ljh_asm_mul_u16),|.text$$lj$$21|
        and     w3, w2, #0xffff
        and     w4, w1, #0xffff
        mul     w3, w4, w3
        cmp     wzr, w3, lsr 16
        strh    w3, [x0]
        cset    w0, ne
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_mul_i32) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_mul_i32),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY_COMDAT A64NAME(_ljh_asm_mul_i32),|.text$$lj$$21|
        smull   x3, w1, w2
        cmp     x3, w3, sxtw
        str     w3, [x0]
        cset    w0, ne
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_mul_u32) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_mul_u32),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY_COMDAT A64NAME(_ljh_asm_mul_u32),|.text$$lj$$21|
        umull   x3, w1, w2
        cmp     xzr, x3, lsr 32
        str     w3, [x0]
        cset    w0, ne
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_mul_i64) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_mul_i64),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY_COMDAT A64NAME(_ljh_asm_mul_i64),|.text$$lj$$21|
        mul     x3, x1, x2
        smulh   x1, x1, x2
        str     x3, [x0]
        cmp     x1, x3, asr 63
        cset    w0, ne
        ret
    LEAF_END

    EXPORT A64NAME(_ljh_asm_mul_u64) [FUNC]
    ARM64EC_ENTRY_THUNK A64NAME(_ljh_asm_mul_u64),1,0,|.text$$lj$$21|,5
    LEAF_ENTRY_COMDAT A64NAME(_ljh_asm_mul_u64),|.text$$lj$$21|
        umulh   x3, x1, x2
        mul     x1, x1, x2
        cmp     x3, 0
        str     x1, [x0]
        cset    w0, ne
        ret
    LEAF_END

    END
