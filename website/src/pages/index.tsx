import React from "react";
import clsx from "clsx";
import Link from "@docusaurus/Link";
import useDocusaurusContext from "@docusaurus/useDocusaurusContext";
import Layout from "@theme/Layout";

import HomepageFeatures from "@site/src/components/HomepageFeatures";
import HomepageSpiritFiles from "@site/src/components/HomepageSpiritFiles";
import HomepageDownloads from "@site/src/components/HomepageDownloads";

import styles from "./index.module.css";

function HomepageHeader() {
  const { siteConfig } = useDocusaurusContext();
  return (
    <header className={clsx("hero hero--primary", styles.heroBanner)}>
      <div className="container">
        <img height="150px" width="auto"
	 src={require("@site/static/img/spirit.png").default} alt="Logo" />
        <h1 className="hero__title">{siteConfig.title}</h1>
        <p className="hero__subtitle">{siteConfig.tagline}</p>
        <div className={styles.buttons}>
          <Link
            className="button button--secondary button--lg"
            to="/docs/Introduction"
          >
            User Manual ⏱️
          </Link>

          <Link
            style={{ margin: "10px" }}
            className="button button--secondary button--lg"
            to=""
            onClick={() => {
              document
                .getElementById("downloads")
                .scrollIntoView({ behavior: "smooth" });
            }}
          >
            Download Now 📥
          </Link>
        </div>
        <br />
        <img
          src={require("@site/static/img/spirit-preview.webp").default}
          alt="Banner"
          style={{
            backgroundImage: `url("${require('@site/static/img/preview-background.png').default}")`,
            backgroundSize: "cover",
            backgroundRepeat: "no-repeat",
          }}
        />
      </div>
    </header>
  );
}

export default function Home(): JSX.Element {
  const { siteConfig } = useDocusaurusContext();
  return (
    <Layout
      title={`${siteConfig.tagline}`}
      description={`Official Website for ${siteConfig.title} | ${siteConfig.tagline}`}
    >
      <HomepageHeader />
      <main>
        <HomepageFeatures />
        <HomepageSpiritFiles />
        <div id="downloads">
          <HomepageDownloads />
        </div>
      </main>
    </Layout>
  );
}
