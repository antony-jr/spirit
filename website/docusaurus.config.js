// @ts-check
// Note: type annotations allow type checking and IDEs autocompletion

const lightCodeTheme = require('prism-react-renderer/themes/github');
const darkCodeTheme = require('prism-react-renderer/themes/dracula');

/** @type {import('@docusaurus/types').Config} */
const config = {
  title: 'Spirit',
  tagline: 'Modern and Hackable Desktop Mascot',
  url: 'https://antonyjr.in/spirit',
  baseUrl: '/',
  onBrokenLinks: 'throw',
  onBrokenMarkdownLinks: 'warn',
  favicon: 'img/favicon.ico',

  // GitHub pages deployment config.
  // If you aren't using GitHub pages, you don't need these.
  organizationName: 'antony-jr', // Usually your GitHub org/user name.
  projectName: 'spirit', // Usually your repo name.

  customFields: {
   links: {
     linux_stable_link_raw: process.env.LINUX_STABLE_LINK,
     linux_dev_link_raw: process.env.LINUX_DEV_LINK,
     mac_stable_link_raw: process.env.MAC_STABLE_LINK,
     mac_dev_link_raw: process.env.MAC_DEV_LINK,
     win_stable_link_raw: process.env.WIN_STABLE_LINK,
     win_dev_link_raw: process.env.WIN_DEV_LINK
   },
  },

  // Even if you don't use internalization, you can use this field to set useful
  // metadata like html lang. For example, if your site is Chinese, you may want
  // to replace "en" with "zh-Hans".
  i18n: {
    defaultLocale: 'en',
    locales: ['en'],
  },

  presets: [
    [
      'classic',
      /** @type {import('@docusaurus/preset-classic').Options} */
      ({
        docs: {
          sidebarPath: require.resolve('./sidebars.js'),
          // Please change this to your repo.
          // Remove this to remove the "edit this page" links.
          //editUrl:
          //  'https://github.com/facebook/docusaurus/tree/main/packages/create-docusaurus/templates/shared/',
        },
        blog: {
          showReadingTime: true,
          // Please change this to your repo.
          // Remove this to remove the "edit this page" links.
          //editUrl:
          //  'https://github.com/facebook/docusaurus/tree/main/packages/create-docusaurus/templates/shared/',
        },
        theme: {
          customCss: require.resolve('./src/css/custom.css'),
        },
      }),
    ],
  ],

  themeConfig:
    /** @type {import('@docusaurus/preset-classic').ThemeConfig} */
    ({
      navbar: {
        title: 'Spirit',
        logo: {
          alt: 'Spirit',
          src: 'img/logo.svg',
        },
        items: [
	  {
	    href: '#downloads',
	    label: 'Downloads',
	    position: 'left',
	  },
	  {
            type: 'doc',
            docId: 'Introduction',
            position: 'left',
            label: 'Documentation',
          },
          {to: '/blog', label: 'Blog', position: 'left'},
          {
            href: 'https://github.com/antony-jr/spirit',
            label: 'GitHub',
            position: 'right',
          },
        ],
      },
      footer: {
        style: 'dark',
        links: [
          {
            title: 'Docs',
            items: [
              {
                label: 'Introduction',
                to: '/docs/Introduction',
              },
	      {
		 label: 'Usage',
		 to: '/docs/category/usage',
	      },
	      {
		label: 'Download Spirit Files',
		to: '/docs/spirit-files/download',
	      }
            ],
          },
          {
            title: 'Community',
            items: [
              {
                label: 'Stack Overflow',
                href: 'https://stackoverflow.com/questions/tagged/spirit',
              },
              /*{
                label: 'Discord',
                href: 'https://discordapp.com/invite/docusaurus',
	      },*/
              {
                label: 'Twitter',
                href: 'https://twitter.com/antonyjr0',
              },
            ],
          },
          {
            title: 'More',
            items: [
              {
                label: 'Blog',
                to: '/blog',
              },
              {
                label: 'GitHub',
                href: 'https://github.com/antony-jr/spirit',
              },
            ],
          },
        ],
        copyright: `Copyright © ${new Date().getFullYear()} D. Antony J.R.`,
      },
      prism: {
        theme: lightCodeTheme,
        darkTheme: darkCodeTheme,
      },
    }),
};

module.exports = config;
