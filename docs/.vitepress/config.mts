import { defineConfig } from "vitepress";
import typedocSidebar from "../reference/typedoc-sidebar.json";
import guideSidebar from "../guide/sidebar.json";

// https://vitepress.dev/reference/site-config
export default defineConfig({
    title: "JS_Engine",
    description: "A Javascript engine for Endstone",
    base: "/Js_Engine/",
    themeConfig: {
        // https://vitepress.dev/reference/default-theme-config
        outline: {
            level: [2, 3],
            label: "On this page",
        },

        nav: [
            { text: "Home", link: "/" },
            { text: "Guide", link: "/guide" },
            { text: "API Reference", link: "/reference" },
        ],

        sidebar: [
            {
                text: "Guide",
                items: guideSidebar,
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
});
