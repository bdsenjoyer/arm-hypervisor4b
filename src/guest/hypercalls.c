// ─────────────────────────────────────────────────────────────────
// hypercalls.c — C-обгортки над HVC інструкцією
//
// HVC (Hypervisor Call) — це як syscall але для гіпервізора.
// Гість виконує "hvc #0", процесор перемикається в EL2,
// гіпервізор дивиться на x0 (номер виклику) і робить потрібне.
//
// Конвенція (як у нашому hypercall.h):
//   x0 = номер hypercall
//   x1, x2 = аргументи (якщо є)
//   x0 після повернення = результат
// ─────────────────────────────────────────────────────────────────

#include "guest.h"
#include "hypercall.h"  // звідти беремо HVC_VERSION, HVC_EXIT тощо

// ── Базовий макрос для виклику HVC ───────────────────────────────
// __asm__ volatile — вбудований асемблер в C
// "hvc #0"         — власне виклик
// "=r"(result)     — результат пишеться в будь-який регістр (r), зберігаємо в result
// "r"(num)         — num передається через регістр
// "x0"–"x2"        — кажемо компілятору що ці регістри змінюються

static inline uint64_t hvc_call(uint64_t num, uint64_t arg1, uint64_t arg2) {
    register uint64_t x0 asm("x0") = num;
    register uint64_t x1 asm("x1") = arg1;
    register uint64_t x2 asm("x2") = arg2;

    __asm__ volatile (
        "hvc #0"
        : "+r"(x0)          // x0 = вхід (номер) і вихід (результат)
        : "r"(x1), "r"(x2)  // x1, x2 = тільки вхід
        : "memory"           // може змінити пам'ять (на всяк)
    );

    return x0;
}

// ── Конкретні hypercall-и ─────────────────────────────────────────

// Версія гіпервізора — повертає uint32 packed: major<<16 | minor
uint64_t hvc_get_version(void) {
    return hvc_call(HVC_VERSION, 0, 0);
}

// Час в мікросекундах з моменту старту гіпервізора
uint64_t hvc_get_time(void) {
    return hvc_call(HVC_GET_TIME, 0, 0);
}

// ID поточного CPU (0 на single-core RPi4 в нашому випадку)
uint64_t hvc_get_cpu_id(void) {
    return hvc_call(HVC_GET_CPU_ID, 0, 0);
}

// Статистика VM — повертає кількість hypercall-ів зроблених досі
uint64_t hvc_vm_stats(void) {
    return hvc_call(HVC_VM_STATS, 0, 0);
}

// Debug print — виводить value через UART гіпервізора
// Зручно для дебагу коли свій UART ще не налаштований
void hvc_debug_print(uint64_t value) {
    hvc_call(HVC_DEBUG_PRINT, value, 0);
}

// Чистий вихід — гість каже гіпервізору "я закінчив"
// Гіпервізор виводить статистику і зупиняється
void hvc_exit(void) {
    hvc_call(HVC_EXIT, 0, 0);
    // Сюди ми не повернемось, але компілятор не знає —
    // тому нижче __builtin_unreachable() щоб не генерував зайвий код
    __builtin_unreachable();
}
