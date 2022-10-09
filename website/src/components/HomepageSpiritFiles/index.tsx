import React from "react";
import Link from "@docusaurus/Link";
import clsx from "clsx";
import styles from "./styles.module.css";

export default function HomepageSpiritFiles(): JSX.Element {
  return (
    <div class="hero  hero--dark">
      <div class="container">
        <h1 class="hero__title">Spirit Files</h1>

        <p class="hero__subtitle">
          <b>"Spirit Files"</b> are compressed zip archives (ends with .spirit)
          which describes a mascot for Spirit to paint in your Desktop, Spirit
          files can have many actions and also include music if the spirit file
          author wishes to. By default Spirit comes with a spirit file embedded
          into it with basic functionality.
          <br /> <br />
          You can{" "}
          <Link to="/docs/category/spirit-file">
            create your own "Spirit File"
          </Link>{" "}
          or download existing one from large collection of spirits{" "}
          <Link to="/docs/spirit-files/download">here</Link>
        </p>

        <center>
          <img
            height="550px"
            width="auto"
            src="/img/linux_program.png"
            alt="Linux Program"
          />
        </center>

        <br />

        <h1 class="hero__title">Spirit Rest API</h1>
        <p class="hero__subtitle">
          Spirit also provides an rest API from which any program running in the
          same machine can communicate with it and can control it. This allows
          anyone to customize Spirit to their wishes. For example you can write
          a small rust program which runs in the background to check for Github
          notifications or Discord messages and use the local API to make the
          mascot do something or play a tune.
          <br /> <br />
          Read the <Link to="/docs/category/rest-api">user manual</Link> to know
          more about the API or checkout some cool applications which extends
          Spirit <Link to="">here</Link>.
        </p>
      </div>
    </div>
  );
}
