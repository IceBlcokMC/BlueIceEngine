import { defineConfig } from "vitepress";
import typedocSidebar from "../reference/typedoc-sidebar.json";
import zh_sidebar from "../zh/guides/sidebar.json";
import en_sidebar from "../en/guides/sidebar.json";

// https://vitepress.dev/reference/site-config
export default defineConfig({
    base: "/Js_Engine/",
    title: "JS_Engine",

    // default en_US
    description: "A Javascript engine for Endstone",
    themeConfig: {
        i18nRouting: true,
        outline: {
            level: [2, 3],
            label: "On this page",
        },
        nav: [
            { text: "Home", link: "/en" },
            { text: "Quick Start", link: "/en/guides/quick_start" },
            { text: "API Reference", link: "/reference" },
        ],
        sidebar: [
            {
                text: "Guides",
                items: en_sidebar,
            },
            {
                text: "API Reference",
                items: typedocSidebar,
            },
        ],
        socialLinks: [
            { icon: "github", link: "https://github.com/IceBlcokMC/Js_Engine" },
        ],
    },

    // https://vitepress.dev/reference/i18n-config
    locales: {
        zh: {
            lang: "zh",
            label: "简体中文",
            link: "/zh",
            description: "适用于 Endstone 的 Javascript 引擎",
            themeConfig: {
                outline: {
                    label: "本页内容",
                },
                nav: [
                    { text: "首页", link: "/zh" },
                    { text: "快速开始", link: "/zh/guides/quick_start" },
                    { text: "API 参考", link: "/reference" },
                ],
                sidebar: [
                    {
                        text: "指南",
                        items: zh_sidebar,
                    },
                    {
                        text: "API 参考",
                        items: typedocSidebar,
                    },
                ],
            },
        },
        root: {
            lang: "en",
            label: "English",
            link: "/en",
        },
    },
});
