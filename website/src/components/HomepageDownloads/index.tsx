import React from "react";
import Link from "@docusaurus/Link";
import clsx from "clsx";
import styles from "./styles.module.css";

import siteConfig from "@generated/docusaurus.config";

type PlatformItem = {
  title: string;
  Svg: React.ComponentType<React.ComponentProps<"svg">>;
  description: JSX.Element;
  stable_version_link: string;
  development_version_link: string;
};

const linux_stable_link =
  siteConfig.customFields.links.linux_stable_link_raw == undefined
    ? "https://github.com/antony-jr/spirit"
    : siteConfig.customFields.links.linux_stable_link_raw;

const linux_dev_link =
  siteConfig.customFields.links.linux_dev_link_raw == undefined
    ? "https://github.com/antony-jr/spirit"
    : siteConfig.customFields.links.linux_dev_link_raw;

const mac_stable_link =
  siteConfig.customFields.links.mac_stable_link_raw == undefined
    ? "https://github.com/antony-jr/spirit"
    : siteConfig.customFields.links.mac_stable_link_raw;

const mac_dev_link =
  siteConfig.customFields.links.mac_dev_link_raw == undefined
    ? "https://github.com/antony-jr/spirit"
    : siteConfig.customFields.links.mac_dev_link_raw;

const win_stable_link =
  siteConfig.customFields.links.win_stable_link_raw == undefined
    ? "https://github.com/antony-jr/spirit"
    : siteConfig.customFields.links.win_stable_link_raw;

const win_dev_link =
  siteConfig.customFields.links.win_dev_link_raw == undefined
    ? "https://github.com/antony-jr/spirit"
    : siteConfig.customFields.links.win_dev_link_raw;

const PlatformList: PlatformItem[] = [
  {
    title: "GNU/Linux Distributions",
    Svg: require("@site/static/img/linux.svg").default,
    description: (
      <>
        AppImages are similar to Disk Images in MacOS and can thus simply "run"
        in all major GNU/Linux distros. See{" "}
        <Link href="https://appimage.org/">AppImage Docs</Link> on how to run
        them. This AppImage will run on{" "}
        <b>distros newer than or same as Ubuntu 18.04 LTS.</b>
      </>
    ),
    stable_version_link: linux_stable_link,
    development_version_link: linux_dev_link,
  },
  {
    title: "Apple macOS",
    Svg: require("@site/static/img/macos.svg").default,
    description: (
      <>
        Open the .dmg file with <b>"Control + Click"</b> and then drag and drop
	 Spirit into the application folder, again <b>"Control + Click"</b> the app bundle
	and select open from the menu. (This is only one time)
      </>
    ),
    stable_version_link: mac_stable_link,
    development_version_link: mac_dev_link,
  },
  {
    title: "Microsoft Windows",
    Svg: require("@site/static/img/windows.svg").default,
    description: (
      <>
        Download the setup file and install the program into your system, please
        avoid warnings from windows, this is because it's not code signed.
      </>
    ),
    stable_version_link: win_stable_link,
    development_version_link: win_dev_link,
  },
];

function Platform({
  title,
  Svg,
  description,
  stable_version_link,
  development_version_link,
}: PlatformItem) {
  return (
    <div className={clsx("col")}>
       <div class="card" style={{minHeight: "550px"}}>
        <div class="card__header">
          <h3>{title}</h3>
        </div>
        <div class="card__body">
          <div className="text--center">
            <Svg className={styles.platformSvg} role="img" />
          </div>

          <br />
          <p>{description}</p>
        </div>
        <div class="card__footer">
          <Link
            target="_self"
            to={stable_version_link}
            style={{ margin: "5px" }}
            class="button button--primary button--block"
          >
            Stable Version
          </Link>
          <Link
            target="_self"
            to={development_version_link}
            style={{ margin: "5px" }}
            class="button button--secondary button--block"
          >
            Development Version
          </Link>
        </div>
      </div>
    </div>
  );
}

export default function HomepageDownloads(): JSX.Element {
  return (
    <div class="hero hero--secondary">
      <div class="container">
        <h1 class="hero__title">Download Now</h1>
        <p class="hero__subtitle">
          Supported on all major platforms, Select one of the options below to
          Download. You can verify the installer's checksum available at Github.
        </p>

        <div className="row">
          {PlatformList.map((props, idx) => (
            <Platform key={idx} {...props} />
          ))}
        </div>
        <br />
        <br />
      </div>
    </div>
  );
}
