import React from "react";
import clsx from "clsx";
import styles from "./styles.module.css";

type FeatureItem = {
  title: string;
  Svg: React.ComponentType<React.ComponentProps<"svg">>;
  description: JSX.Element;
};

const FeatureList: FeatureItem[] = [
  {
    title: "Cross Platform",
    Svg: require("@site/static/img/cross_platform.svg").default,
    description: (
      <>
        This application runs on Windows, MacOS Catalina or later, any GNU/Linux
        distribution released after or at the same time as Ubuntu 18.04 LTS.
      </>
    ),
  },
  {
    title: "Native Performance",
    Svg: require("@site/static/img/fast.svg").default,
    description: (
      <>
        This application is written in C++ using Qt5 Framework, which means the
        applications will be super fast and smooth.
      </>
    ),
  },
  {
    title: "Easy Installation",
    Svg: require("@site/static/img/easy.svg").default,
    description: (
      <>
        Easy installation and integration with all supported platforms, no need
        to install additional software like JRE or anything.
      </>
    ),
  },
  {
    title: "Hackable",
    Svg: require("@site/static/img/hackable.svg").default,
    description: (
      <>
        Spirit provides a local rest api to control and customize every aspect
        of the mascot displayed in the desktop.
      </>
    ),
  },
  {
    title: "Modern",
    Svg: require("@site/static/img/modern.svg").default,
    description: (
      <>
        No need to chase legacy code or instructions to use this software, this
        is a new software built from ground up. Feel free to ask for a feature
        at Github.
      </>
    ),
  },
  {
    title: "Limited Wayland Support",
    Svg: require("@site/static/img/wayland.svg").default,
    description: (
      <>
        Limited Wayland support for GNOME Shell. We will try to support more DEs
        which uses wayland to the best of our efforts.
      </>
    ),
  },
];

function Feature({ title, Svg, description }: FeatureItem) {
  return (
    <div className={clsx("col col--4")}>
      <div className="text--center">
        <Svg className={styles.featureSvg} role="img" />
      </div>
      <div className="text--center padding-horiz--md">
        <h3>{title}</h3>
        <p>{description}</p>
      </div>
    </div>
  );
}

export default function HomepageFeatures(): JSX.Element {
  return (
    <section className={styles.features}>
      <div className="container">
        <h1 className="hero__title" style={{ textAlign: "center" }}>
          Top Features
        </h1>
        <div className="row">
          {FeatureList.map((props, idx) => (
            <Feature key={idx} {...props} />
          ))}
        </div>
      </div>
    </section>
  );
}
