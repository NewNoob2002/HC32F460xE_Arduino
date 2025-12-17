#ifndef SYSTEM_INFOS_VIEW_H
#define SYSTEM_INFOS_VIEW_H

#include "../Page.h"

namespace Page {
    class SystemInfosView {
    public:
        SystemInfosView() {
        }

        ~SystemInfosView() {
        }

        void Create(lv_obj_t *root);

        void Delete();

        typedef struct {
            lv_obj_t *cont;
            lv_obj_t *icon;
            lv_obj_t *labelInfo;
            lv_obj_t *labelData;
        } item_t;

        struct {
            item_t work;
            item_t gps;
            item_t wifi;
            item_t battery;
            item_t storage;
            item_t system;
        } ui;

        void SetWork(
            WorkMode_t workMode,
            RadioInfo_t radioInfo,
            const NtripInfo_t &ntripInfo
        )const;

        void SetGPS(
            double lat,
            double lng,
            double alt
        ) const;

        void SetWifi(const WifiInfo_t &wifiInfo) const;

        void SetBattery(const BatteryInfo_t &batteryInfo) const;

        void SetStorage(const RecordInfo_t &recordInfo) const;

        void SetSystem(
            const char *firmVer,
            const char *bootTime,
            const char *compilerName,
            const char *buildTime
        ) const;

        static void SetScrollToY(lv_obj_t *obj, lv_coord_t y, lv_anim_enable_t en);

        static void onFocus(lv_group_t *e);

    private:
        struct {
            lv_style_t icon;
            lv_style_t focus;
            lv_style_t info;
            lv_style_t data;
        } style;

    private:
        void Group_Init();

        void Style_Init();

        void Style_Reset();

        void Item_Create(
            item_t *item,
            lv_obj_t *par,
            const char *name,
            const char *img_src,
            const char *infos
        );
    };
}

#endif // SYSTEM_INFOS_VIEW_H
