#include "StarMapView.h"
#include <cstdio>

using namespace Page;

namespace {
constexpr lv_color_t kBackground = LV_COLOR_MAKE(0x15, 0x15, 0x13);
constexpr lv_color_t kLineGrey = LV_COLOR_MAKE(0x72, 0x6E, 0x63);
constexpr lv_color_t kLineDark = LV_COLOR_MAKE(0x33, 0x33, 0x30);
constexpr lv_color_t kTextWarm = LV_COLOR_MAKE(0xE8, 0xE1, 0xCF);

// Constellation Colors
constexpr lv_color_t kColorGps = LV_COLOR_MAKE(0xE1, 0xAA, 0x22); // Amber/Yellow
constexpr lv_color_t kColorBds = LV_COLOR_MAKE(0x34, 0x98, 0xDB); // Blue
constexpr lv_color_t kColorGln = LV_COLOR_MAKE(0x2E, 0xCC, 0x71); // Green
constexpr lv_color_t kColorGal = LV_COLOR_MAKE(0xE7, 0x4C, 0x3C); // Red
constexpr lv_color_t kColorSba = LV_COLOR_MAKE(0x9B, 0x59, 0xB6); // Purple
constexpr lv_color_t kColorQzs = LV_COLOR_MAKE(0x1A, 0xBC, 0x9C); // Cyan/Teal
constexpr lv_color_t kColorIrn = LV_COLOR_MAKE(0xE6, 0x7E, 0x22); // Orange

// Statically scattered dot coordinates relative to radar center (40, 40)
const int dot_offsets_x[28] = {
    // GPS (0-3)
    -25, 15, -10, 30,
    // BDS (4-7)
    -30, 20, -5, 25,
    // GLONASS (8-11)
    -18, 12, -28, 32,
    // GALILEO (12-15)
    -12, 5, 28, -22,
    // SBAS (16-19)
    -5, 22, -32, 8,
    // QZSS (20-23)
    -15, 28, -20, 10,
    // IRNSS (24-27)
    -25, 5, 25, -12};

const int dot_offsets_y[28] = {
    // GPS (0-3)
    -20, -30, 25, 10,
    // BDS (4-7)
    15, -15, -28, 28,
    // GLONASS (8-11)
    -12, 20, -25, -20,
    // GALILEO (12-15)
    32, -8, -5, 22,
    // SBAS (16-19)
    12, -22, 5, 30,
    // QZSS (20-23)
    -30, 18, 15, -18,
    // IRNSS (24-27)
    -5, 25, -25, -22};
} // namespace

void
StarMapView::Create(lv_obj_t* root) {
    // 1. Root Container Style
    lv_obj_set_size(root, 294, 100);
    lv_obj_set_align(root, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_style_bg_color(root, kBackground, 0);
    lv_obj_set_style_bg_opa(root, LV_OPA_COVER, 0);
    lv_obj_clear_flag(root, LV_OBJ_FLAG_SCROLLABLE);

    const lv_font_t* font_title = ResourcePool::GetFont("oswaldBold_18");
    const lv_font_t* font_small = ResourcePool::GetFont("oswaldBold_12");

    // 2. Radar Background Circle
    lv_obj_t* radar_bg = lv_obj_create(root);
    lv_obj_remove_style_all(radar_bg);
    lv_obj_set_size(radar_bg, 80, 80);
    lv_obj_align(radar_bg, LV_ALIGN_LEFT_MID, 15, 0);
    lv_obj_set_style_radius(radar_bg, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_border_color(radar_bg, kLineGrey, 0);
    lv_obj_set_style_border_width(radar_bg, 1, 0);
    lv_obj_set_style_bg_opa(radar_bg, LV_OPA_TRANSP, 0);
    ui.radar_bg = radar_bg;

    // 3. Radar Inner Concentric Circle
    lv_obj_t* radar_inner = lv_obj_create(radar_bg);
    lv_obj_remove_style_all(radar_inner);
    lv_obj_set_size(radar_inner, 44, 44);
    lv_obj_align(radar_inner, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_radius(radar_inner, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_border_color(radar_inner, kLineDark, 0);
    lv_obj_set_style_border_width(radar_inner, 1, 0);
    lv_obj_set_style_bg_opa(radar_inner, LV_OPA_TRANSP, 0);
    ui.radar_inner = radar_inner;

    // 4. Radar Crosshairs
    lv_obj_t* line_h = lv_obj_create(radar_bg);
    lv_obj_remove_style_all(line_h);
    lv_obj_set_size(line_h, 80, 1);
    lv_obj_align(line_h, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(line_h, kLineDark, 0);
    lv_obj_set_style_bg_opa(line_h, LV_OPA_COVER, 0);
    ui.line_h = line_h;

    lv_obj_t* line_v = lv_obj_create(radar_bg);
    lv_obj_remove_style_all(line_v);
    lv_obj_set_size(line_v, 1, 80);
    lv_obj_align(line_v, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(line_v, kLineDark, 0);
    lv_obj_set_style_bg_opa(line_v, LV_OPA_COVER, 0);
    ui.line_v = line_v;

    // 5. Create 28 Satellite Dots
    for (int i = 0; i < 28; ++i) {
        lv_obj_t* dot = lv_obj_create(radar_bg);
        lv_obj_remove_style_all(dot);
        lv_obj_set_size(dot, 4, 4);
        lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);

        // Color coding by index
        lv_color_t c = kColorGps;
        if (i >= 4 && i < 8) {
            c = kColorBds;
        } else if (i >= 8 && i < 12) {
            c = kColorGln;
        } else if (i >= 12 && i < 16) {
            c = kColorGal;
        } else if (i >= 16 && i < 20) {
            c = kColorSba;
        } else if (i >= 20 && i < 24) {
            c = kColorQzs;
        } else if (i >= 24) {
            c = kColorIrn;
        }

        lv_obj_set_style_bg_color(dot, c, 0);

        // Coordinate positioning
        int x = 40 + dot_offsets_x[i] - 2;
        int y = 40 + dot_offsets_y[i] - 2;
        lv_obj_set_pos(dot, x, y);

        // Hide initially
        lv_obj_add_flag(dot, LV_OBJ_FLAG_HIDDEN);
        ui.dots[i] = dot;
    }

    // 6. Right Side Information Panel
    lv_obj_t* info_cont = lv_obj_create(root);
    lv_obj_remove_style_all(info_cont);
    lv_obj_set_size(info_cont, 175, 80);
    lv_obj_align(info_cont, LV_ALIGN_LEFT_MID, 110, 0);
    ui.info_cont = info_cont;

    // Title label
    lv_obj_t* label_title = lv_label_create(info_cont);
    lv_obj_remove_style_all(label_title);
    lv_obj_set_style_text_font(label_title, font_title, 0);
    lv_obj_set_style_text_color(label_title, kTextWarm, 0);
    lv_obj_set_style_text_letter_space(label_title, 1, 0);
    lv_obj_align(label_title, LV_ALIGN_TOP_LEFT, 0, 0);
    ui.label_title = label_title;

    // 2x4 Grid for 7 Constellation Counts (8 slots, last one unused)
    const char* names[7] = {"GPS", "BDS", "GLN", "GAL", "SBA", "QZS", "IRN"};
    lv_color_t colors[7] = {kColorGps, kColorBds, kColorGln, kColorGal, kColorSba, kColorQzs, kColorIrn};

    for (int i = 0; i < 7; ++i) {
        int row = i / 2;
        int col = i % 2;
        int x_base = col * 90;
        // Shift starting Y down to Y=24 to leave a small gap below the title
        int y_base = 24 + row * 14;

        // Container/cont for item
        lv_obj_t* cont = lv_obj_create(info_cont);
        lv_obj_remove_style_all(cont);
        lv_obj_set_size(cont, 85, 14);
        lv_obj_set_pos(cont, x_base, y_base);
        ui.constell[i].cont = cont;

        // Dot indicator
        lv_obj_t* dot = lv_obj_create(cont);
        lv_obj_remove_style_all(dot);
        lv_obj_set_size(dot, 6, 6);
        lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_bg_color(dot, colors[i], 0);
        lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
        lv_obj_align(dot, LV_ALIGN_LEFT_MID, 0, 0);
        ui.constell[i].dot = dot;

        // Name
        lv_obj_t* label_name = lv_label_create(cont);
        lv_obj_remove_style_all(label_name);
        lv_obj_set_style_text_font(label_name, font_small, 0);
        lv_obj_set_style_text_color(label_name, kLineGrey, 0);
        lv_label_set_text(label_name, names[i]);
        lv_obj_align_to(label_name, dot, LV_ALIGN_OUT_RIGHT_MID, 6, -1);
        ui.constell[i].label_name = label_name;

        // Value
        lv_obj_t* label_val = lv_label_create(cont);
        lv_obj_remove_style_all(label_val);
        lv_obj_set_style_text_font(label_val, font_small, 0);
        lv_obj_set_style_text_color(label_val, kTextWarm, 0);
        lv_label_set_text(label_val, "0");
        // Align value label to the right side of the cell to prevent overlap
        lv_obj_align(label_val, LV_ALIGN_RIGHT_MID, -5, 0);
        ui.constell[i].label_val = label_val;
    }

    // Blink dot to indicate onTimerUpdate
    lv_obj_t* blink_dot = lv_obj_create(root);
    lv_obj_remove_style_all(blink_dot);
    lv_obj_set_size(blink_dot, 5, 5);
    lv_obj_set_style_radius(blink_dot, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(blink_dot, kColorGln, 0);
    lv_obj_set_style_bg_opa(blink_dot, LV_OPA_COVER, 0);
    lv_obj_align(blink_dot, LV_ALIGN_TOP_RIGHT, -10, 10);
    ui.blink_dot = blink_dot;

    ApplyLanguage();
}

void
StarMapView::Delete() {}

void
StarMapView::UpdateStatusIndicator() {
    if (ui.blink_dot) {
        if (is_blink_dot_changed) {
            lv_obj_set_style_bg_color(ui.blink_dot, kColorGln, 0);
        } else {
            lv_obj_set_style_bg_color(ui.blink_dot, kColorGal, 0);
        }
        is_blink_dot_changed = !is_blink_dot_changed;
    }
}

void
StarMapView::ApplyLanguage() const {
    lv_label_set_text(ui.label_title, I18n::Text(I18n::TextId::StarMapTitle));
    lv_label_set_text(ui.constell[0].label_name, I18n::Text(I18n::TextId::StarMapGps));
    lv_label_set_text(ui.constell[1].label_name, I18n::Text(I18n::TextId::StarMapBds));
    lv_label_set_text(ui.constell[2].label_name, I18n::Text(I18n::TextId::StarMapGlonass));
    lv_label_set_text(ui.constell[3].label_name, I18n::Text(I18n::TextId::StarMapGalileo));
    lv_label_set_text(ui.constell[4].label_name, I18n::Text(I18n::TextId::StarMapSbas));
    lv_label_set_text(ui.constell[5].label_name, I18n::Text(I18n::TextId::StarMapQzss));
    lv_label_set_text(ui.constell[6].label_name, I18n::Text(I18n::TextId::StarMapIrnss));
}

void
StarMapView::UpdateValues(int gps, int bds, int gln, int gal, int sbas, int qzss, int irnss) {
    char buf[16];

    // Update value labels
    std::snprintf(buf, sizeof(buf), "%d", gps);
    lv_label_set_text(ui.constell[0].label_val, buf);

    std::snprintf(buf, sizeof(buf), "%d", bds);
    lv_label_set_text(ui.constell[1].label_val, buf);

    std::snprintf(buf, sizeof(buf), "%d", gln);
    lv_label_set_text(ui.constell[2].label_val, buf);

    std::snprintf(buf, sizeof(buf), "%d", gal);
    lv_label_set_text(ui.constell[3].label_val, buf);

    std::snprintf(buf, sizeof(buf), "%d", sbas);
    lv_label_set_text(ui.constell[4].label_val, buf);

    std::snprintf(buf, sizeof(buf), "%d", qzss);
    lv_label_set_text(ui.constell[5].label_val, buf);

    std::snprintf(buf, sizeof(buf), "%d", irnss);
    lv_label_set_text(ui.constell[6].label_val, buf);

    // Update dot visibility on radar
    // Calculate how many dots to show for each constellation based on counts
    int counts[7] = {gps, bds, gln, gal, sbas, qzss, irnss};
    for (int c = 0; c < 7; ++c) {
        int used = counts[c];
        int show_dots = 0;
        if (used > 0) {
            show_dots = (used + 1) / 2; // e.g. 1 used -> 1 dot, 3 used -> 2 dots, 5 used -> 3 dots, >=7 used -> 4 dots
            if (show_dots > 4) {
                show_dots = 4;
            }
        }

        for (int d = 0; d < 4; ++d) {
            int idx = c * 4 + d;
            if (d < show_dots) {
                lv_obj_clear_flag(ui.dots[idx], LV_OBJ_FLAG_HIDDEN);
            } else {
                lv_obj_add_flag(ui.dots[idx], LV_OBJ_FLAG_HIDDEN);
            }
        }
    }
}
