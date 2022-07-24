﻿using Nuke.Common;

namespace MaaBuilder;

public partial class Build
{
    /// <summary>
    /// 见 <see cref="ActionConfiguration.DevBuild"/>
    /// </summary>
    Target DevBuild => _ => _
        .DependsOn(UseCommitVersion)
        .DependsOn(UseMaaDevBundle)
        .DependsOn(UsePublishArtifact);
        

    /// <summary>
    /// 见 <see cref="ActionConfiguration.ReleaseMaa"/>
    /// </summary>
    Target ReleaseMaa => _ => _
        .DependsOn(UseTagVersion)
        .DependsOn(UseMaaRelease)
        .DependsOn(UseMaaChangeLog)
        .DependsOn(UsePublishArtifact)
        .DependsOn(UsePublishRelease);
}
