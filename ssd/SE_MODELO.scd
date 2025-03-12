<?xml version="1.0" encoding="utf-8"?>
<SCL version="2007" revision="B" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.iec.ch/61850/2003/SCL SCL.xsd" xmlns="http://www.iec.ch/61850/2003/SCL">
  <Private type="SchneiderElectric-SET-SCLIPV4Plan">
    <SCLIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" Deployed="No">
      <IPAddress IsWarning="False" IsError="None">
        <Octet_1 PartValue="192" IsLocal="False" />
        <Octet_2 PartValue="168" IsLocal="False" />
        <Octet_3 PartValue="0:255" IsLocal="False" />
        <Octet_4 PartValue="1:255" IsLocal="False" />
      </IPAddress>
      <Subnetmask IsWarning="False" IsError="None">
        <Octet_1 PartValue="255" IsLocal="False" />
        <Octet_2 PartValue="255" IsLocal="False" />
        <Octet_3 PartValue="255" IsLocal="False" />
        <Octet_4 PartValue="0" IsLocal="False" />
      </Subnetmask>
      <Gateway IsWarning="False" IsError="None">
        <Octet_1 PartValue="0" IsLocal="False" />
        <Octet_2 PartValue="0" IsLocal="False" />
        <Octet_3 PartValue="0" IsLocal="False" />
        <Octet_4 PartValue="0" IsLocal="False" />
      </Gateway>
    </SCLIPV4PlanPrivates>
  </Private>
  <Private type="SchneiderElectric-SET-AutoNaming">
    <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
  </Private>
  <Private type="SchneiderElectric-SET-WorkSpace">
    <WorkSpacePrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
      <WorkSpaceAreas>
        <WorkSpaceArea WorkSpaceName="Workspace" WorkAreaIdentification="00000000-0000-0000-0000-000000000001" EquipmentOpacityFactor="1" TopologyOpacityFactor="1" ElectricalContainerOpacityFactor="1" SLDOpacityFactor="1" LayerZoomFactor="0.512" ScaleFactor="1" DPI="0" Opacity="1" HasImage="False" HideEquipments="False" ElementsLocked="True" DisplayErrors="False" DisplayPEName="True">
          <LayerInnerPosition X="251.101686835593" Y="-291.76907219012" />
          <LayerOuterPosition X="233" Y="253" />
        </WorkSpaceArea>
      </WorkSpaceAreas>
    </WorkSpacePrivates>
  </Private>
  <Private type="SchneiderElectric-SET-Project-Export">
    <ProjectExportPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
      <SCLVersions>
        <String>2007B</String>
      </SCLVersions>
    </ProjectExportPrivates>
  </Private>
  <Private type="SchneiderElectric-SET-ControlManagementData">
    <ControlManagementPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" SBOTimeOUT="-1" SBOClass="Undefined" CTLModel="Undefined" />
  </Private>
  <Private type="SchneiderElectric-SET-Deploy-LNode-SpecNaming">
    <DeployLNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
  </Private>
  <Private type="SchneiderElectric-SET-Ignore-NamPlt">
    <IgnoreNamPltComparasonPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="True" />
  </Private>
  <Header id="SE Modelo CEMIG T" version="2" revision="1" toolID="SchneiderElectric-SET" nameStructure="IEDName" />
  <Substation name="SEMODELO" desc="">
    <Private type="SchneiderElectric-SET-IEDAssociations">
      <IEDAssociationPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <IEDsAssociatedCollection>
          <IEDIdentification IEDName="U_CENTRAL" IsAssociated="True" />
        </IEDsAssociatedCollection>
      </IEDAssociationPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-UserInterface">
      <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <ElectricalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="2727.1064453125" Height="2644.45935058594" />
              <Position X="-493.697692871094" Y="-464.598846435547" />
            </GraphicalProperties>
          </WorkArea>
        </ElectricalWorkAreas>
        <SystemWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000002">
            <GraphicalProperties DuplicateCount="1" IsPlaced="False" IsExpanded="False">
              <Size Width="0" Height="0" />
              <Position X="0" Y="0" />
            </GraphicalProperties>
          </WorkArea>
        </SystemWorkAreas>
        <LogicalWorkAreas />
      </MetaInfosPrivates>
    </Private>
    <LNode iedName="U_CENTRAL" ldInst="LD0" lnClass="IHMI" lnInst="1" lnType="SE_IHMI_SET_default_V001">
      <Private type="SchneiderElectric-SET-LNode-SpecNaming">
        <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
      </Private>
    </LNode>
    <LNode iedName="U_CENTRAL" ldInst="LD0" lnClass="ITCI" lnInst="1" lnType="SE_ITCI_SET_default_V001">
      <Private type="SchneiderElectric-SET-LNode-SpecNaming">
        <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
      </Private>
    </LNode>
    <VoltageLevel name="U" desc="">
      <Private type="SchneiderElectric-SET-AutoNaming">
        <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
      </Private>
      <Private type="SchneiderElectric-SET-IEDAssociations">
        <IEDAssociationPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
          <IEDsAssociatedCollection />
        </IEDAssociationPrivates>
      </Private>
      <Private type="SchneiderElectric-SET-UserInterface">
        <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" ColorProperty="#FF00FF00">
          <ElectricalWorkAreas>
            <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
              <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                <Size Width="1037.41945926379" Height="2199.77791287707" />
                <Position X="-484.793692871094" Y="-434.982846435547" />
              </GraphicalProperties>
            </WorkArea>
          </ElectricalWorkAreas>
          <SystemWorkAreas />
          <LogicalWorkAreas />
        </MetaInfosPrivates>
      </Private>
      <Voltage unit="V" multiplier="k">500</Voltage>
      <Bay name="4U" desc="LT 1">
        <Private type="SchneiderElectric-SET-IEDAssociations">
          <IEDAssociationPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
            <IEDsAssociatedCollection>
              <IEDIdentification IEDName="PATIO_4U3" IsAssociated="True" />
              <IEDIdentification IEDName="PATIO_4U4" IsAssociated="True" />
              <IEDIdentification IEDName="PATIO_4U5" IsAssociated="True" />
              <IEDIdentification IEDName="RELE_P_LT1" IsAssociated="True" />
              <IEDIdentification IEDName="RELE_S_LT1" IsAssociated="True" />
              <IEDIdentification IEDName="RELE_BF_4U_P" IsAssociated="True" />
              <IEDIdentification IEDName="PATIO_4U8" IsAssociated="True" />
              <IEDIdentification IEDName="RELE_BF_4U_S" IsAssociated="True" />
              <IEDIdentification IEDName="UAC_4U_P" IsAssociated="True" />
              <IEDIdentification IEDName="UAC_4U_S" IsAssociated="True" />
              <IEDIdentification IEDName="RDP_4U" IsAssociated="True" />
            </IEDsAssociatedCollection>
          </IEDAssociationPrivates>
        </Private>
        <Private type="SchneiderElectric-SET-UserInterface">
          <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
            <ElectricalWorkAreas>
              <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                  <Size Width="380.742458767361" Height="640.322687092915" />
                  <Position X="-472.978808241094" Y="-309.767306735547" />
                </GraphicalProperties>
              </WorkArea>
            </ElectricalWorkAreas>
            <SystemWorkAreas />
            <LogicalWorkAreas />
          </MetaInfosPrivates>
        </Private>
        <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_MED_" lnClass="MMXU" lnInst="1" lnType="MMXU_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RDP_4U" ldInst="LD0" prefix="RDP_" lnClass="RDRE" lnInst="1" lnType="RDRE_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RDP_4U" ldInst="LD0" prefix="RDP_" lnClass="RBDR" lnInst="1" lnType="RBDR_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RDP_4U" ldInst="LD0" prefix="RDP_" lnClass="RADR" lnInst="1" lnType="RADR_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_Z1_" lnClass="PDIS" lnInst="1" lnType="PDIS_CEMIG" desc="Prot distancia princ Zona 1">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_Z2_" lnClass="PDIS" lnInst="1" lnType="PDIS_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_Z3_" lnClass="PDIS" lnInst="1" lnType="PDIS_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_ZP_" lnClass="PDIS" lnInst="1" lnType="PDIS_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_Z1_" lnClass="PDIS" lnInst="1" lnType="PDIS_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_Z2_" lnClass="PDIS" lnInst="1" lnType="PDIS_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_Z3_" lnClass="PDIS" lnInst="1" lnType="PDIS_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_ZP_" lnClass="PDIS" lnInst="1" lnType="PDIS_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_86B_" lnClass="CILO" lnInst="1" lnType="CILO_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_86TT_" lnClass="CILO" lnInst="1" lnType="CILO_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_86BF_" lnClass="CILO" lnInst="1" lnType="CILO_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_GRP_AL_" lnClass="CALH" lnInst="1" lnType="CALH_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_MED_" lnClass="MMXU" lnInst="1" lnType="MMXU_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_GRP_AL_" lnClass="CALH" lnInst="1" lnType="CALH_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_86TT_" lnClass="CILO" lnInst="1" lnType="CILO_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_86BF_" lnClass="CILO" lnInst="1" lnType="CILO_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_86B_" lnClass="CILO" lnInst="1" lnType="CILO_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_79_" lnClass="RREC" lnInst="1" lnType="RREC_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_79_" lnClass="RREC" lnInst="1" lnType="RREC_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_94_" lnClass="PTRC" lnInst="1" lnType="PTRC_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_50_" lnClass="PIOC" lnInst="1" lnType="PIOC_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_SOTF_" lnClass="PSOF" lnInst="1" lnType="PSOF_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_59_" lnClass="PTOV" lnInst="1" lnType="PTOV_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_25_" lnClass="RSYN" lnInst="1" lnType="RSYN_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_68_" lnClass="RPSB" lnInst="1" lnType="RPSB_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_TLP_" lnClass="PSCH" lnInst="1" lnType="PSCH_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_78_" lnClass="PPAM" lnInst="1" lnType="PPAM_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_67N_" lnClass="PTOC" lnInst="1" lnType="PTOC_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_51_" lnClass="PTOC" lnInst="1" lnType="PTOC_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_50_" lnClass="PIOC" lnInst="1" lnType="PIOC_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_51_" lnClass="PTOC" lnInst="1" lnType="PTOC_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_67N_" lnClass="PTOC" lnInst="1" lnType="PTOC_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_78_" lnClass="PPAM" lnInst="1" lnType="PPAM_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_SOTF_" lnClass="PSOF" lnInst="1" lnType="PSOF_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_59_" lnClass="PTOV" lnInst="1" lnType="PTOV_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_94_" lnClass="PTRC" lnInst="1" lnType="PTRC_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_TLP_" lnClass="PSCH" lnInst="1" lnType="PSCH_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_68_" lnClass="RPSB" lnInst="1" lnType="RPSB_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_25_" lnClass="RSYN" lnInst="1" lnType="RSYN_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_79_5U4" lnClass="RREC" lnInst="1" lnType="RREC_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <ConductingEquipment type="DIS" name="4U5" desc="">
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="None" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-429.310640007344" Y="171.029939460078" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode iedName="PATIO_4U5" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" lnType="XSWI_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_AL_4U5_" lnClass="CALH" lnInst="1" lnType="CALH_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_CO_4U5_" lnClass="CSWI" lnInst="1" lnType="CSWI_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_IT_4U5_" lnClass="CILO" lnInst="1" lnType="CILO_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_AL_4U5_" lnClass="CALH" lnInst="1" lnType="CALH_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_CO_4U5_" lnClass="CSWI" lnInst="1" lnType="CSWI_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_IT_4U5_" lnClass="CILO" lnInst="1" lnType="CILO_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <Terminal name="TERMINAL1" connectivityNode="SEMODELO/U/4U/4U-CON1" substationName="SEMODELO" voltageLevelName="U" bayName="4U" cNodeName="4U-CON1">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/4U/CN2" substationName="SEMODELO" voltageLevelName="U" bayName="4U" cNodeName="CN2">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Top" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="DIS" name="4U3" desc="">
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="None" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-429.499671847344" Y="-257.101880539922" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode iedName="PATIO_4U3" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" lnType="XSWI_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_CO_4U3_" lnClass="CSWI" lnInst="1" lnType="CSWI_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_IT_4U3_" lnClass="CILO" lnInst="1" lnType="CILO_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_AL_4U3_" lnClass="CALH" lnInst="1" lnType="CALH_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_AL_4U3_" lnClass="CALH" lnInst="1" lnType="CALH_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_CO_4U3_" lnClass="CSWI" lnInst="1" lnType="CSWI_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_IT_4U3_" lnClass="CILO" lnInst="1" lnType="CILO_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/4U/CN1" substationName="SEMODELO" voltageLevelName="U" bayName="4U" cNodeName="CN1">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
          <Terminal name="TERMINAL1" connectivityNode="SEMODELO/U/BU1/1BU" substationName="SEMODELO" voltageLevelName="U" bayName="BU1" cNodeName="1BU">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Top" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="CBR" name="4U4" desc="">
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="None" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-429.719994567344" Y="-113.668752379922" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode iedName="PATIO_4U4" ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" lnType="XCBR_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_AL_4U4_" lnClass="CALH" lnInst="1" lnType="CALH_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_CO_4U4_" lnClass="CSWI" lnInst="1" lnType="CSWI_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_IT_4U4_" lnClass="CILO" lnInst="1" lnType="CILO_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_IT_4U4_" lnClass="CILO" lnInst="1" lnType="CILO_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_AL_4U4_" lnClass="CALH" lnInst="1" lnType="CALH_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_CO_4U4_" lnClass="CSWI" lnInst="1" lnType="CSWI_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="RELE_BF_4U_S" ldInst="LD0" prefix="PS_BF94_" lnClass="PTRC" lnInst="1" lnType="PTRC_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="RELE_BF_4U_P" ldInst="LD0" prefix="PP_BF94_" lnClass="PTRC" lnInst="1" lnType="PTRC_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="RELE_BF_4U_S" ldInst="LD0" prefix="PS_BF_" lnClass="RBRF" lnInst="1" lnType="RBRF_CEMIG" desc="Falha disjuntor">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="RELE_BF_4U_P" ldInst="LD0" prefix="PP_BF_" lnClass="RBRF" lnInst="1" lnType="RBRF_CEMIG" desc="Falha disjuntor">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <Terminal name="TERMINAL1" connectivityNode="SEMODELO/U/4U/CN1" substationName="SEMODELO" voltageLevelName="U" bayName="4U" cNodeName="CN1">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Top" />
            </Private>
          </Terminal>
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/4U/CN0" substationName="SEMODELO" voltageLevelName="U" bayName="4U" cNodeName="CN0">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="LIN" name="LT1" desc="">
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="Bottom" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-314.263484364199" Y="-237.949994668027" />
                    <RelativeLabelPosition X="-71" Y="-31.5" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode prefix="" lnClass="ZLIN" lnInst="1" lnType="SE_ZLIN_SET_default_V001" />
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/4U/4U-CON2" substationName="SEMODELO" voltageLevelName="U" bayName="4U" cNodeName="4U-CON2">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="DIS" name="4U8" desc="">
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="None" />
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-315.715838036991" Y="170.729021751956" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode iedName="PATIO_4U8" ldInst="LD0" prefix="PATIO_4U8_" lnClass="XSWI" lnInst="1" lnType="XSWI_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="PATIO_" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_CO_4U8_" lnClass="CSWI" lnInst="1" lnType="CSWI_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_AL_4U8_" lnClass="CALH" lnInst="1" lnType="CALH_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_CO_4U8_" lnClass="CSWI" lnInst="1" lnType="CSWI_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_IT_4U8_" lnClass="CILO" lnInst="1" lnType="CILO_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_AL_4U8_" lnClass="CALH" lnInst="1" lnType="CALH_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_IT_4U8_" lnClass="CILO" lnInst="1" lnType="CILO_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/4U/4U-CON1" substationName="SEMODELO" voltageLevelName="U" bayName="4U" cNodeName="4U-CON1">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
          <Terminal name="TERMINAL1" connectivityNode="SEMODELO/U/4U/4U-CON2" substationName="SEMODELO" voltageLevelName="U" bayName="4U" cNodeName="4U-CON2">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Top" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="DIS" name="4U8T" desc="">
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="Left" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-209.698056786991" Y="93.4674280019556" />
                    <RelativeLabelPosition X="-80.25" Y="101.875" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode iedName="PATIO_4U8" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" lnType="XSWI_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_IT_4U8T_" lnClass="CILO" lnInst="1" lnType="CILO_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_CO_4U8T_" lnClass="CSWI" lnInst="1" lnType="CSWI_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_P" ldInst="LD0" prefix="CP_AL_4U8T_" lnClass="CALH" lnInst="1" lnType="CALH_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_AL_4U8T_" lnClass="CALH" lnInst="1" lnType="CALH_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_IT_4U8T_" lnClass="CILO" lnInst="1" lnType="CILO_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <LNode iedName="UAC_4U_S" ldInst="LD0" prefix="CS_CO_4U8T_" lnClass="CSWI" lnInst="1" lnType="CSWI_CEMIG">
            <Private type="SchneiderElectric-SET-LNode-SpecNaming">
              <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
            </Private>
          </LNode>
          <Terminal connectivityNode="SEMODELO/U/4U/grounded" substationName="SEMODELO" voltageLevelName="U" bayName="4U" cNodeName="grounded" />
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/4U/4U-CON2" substationName="SEMODELO" voltageLevelName="U" bayName="4U" cNodeName="4U-CON2">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Left" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="CTR" name="4UTC" desc="">
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="None" />
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-428.421718896365" Y="23.0686218496116" />
                    <RelativeLabelPosition X="-5" Y="32.5" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode prefix="" lnClass="TCTR" lnInst="1" lnType="SE_TCTR_SET_default_V001" />
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/4U/CN0" substationName="SEMODELO" voltageLevelName="U" bayName="4U" cNodeName="CN0">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Top" />
            </Private>
          </Terminal>
          <Terminal name="TERMINAL1" connectivityNode="SEMODELO/U/4U/CN2" substationName="SEMODELO" voltageLevelName="U" bayName="4U" cNodeName="CN2">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="VTR" name="4UTP" desc="">
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="Left" />
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-211.266562036014" Y="-56.730809302733" />
                    <RelativeLabelPosition X="-76.875" Y="99.6875" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode prefix="" lnClass="TVTR" lnInst="1" lnType="SE_TVTR_SET_default_V001" />
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/4U/4U-CON2" substationName="SEMODELO" voltageLevelName="U" bayName="4U" cNodeName="4U-CON2">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Left" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConnectivityNode pathName="SEMODELO/U/4U/CN1" name="CN1" />
        <ConnectivityNode pathName="SEMODELO/U/4U/4U-CON1" name="4U-CON1" desc="">
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-ConnectivityNode">
            <ConnectivityNodePrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" BusbarType="Point" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="30" Height="25" />
                    <Position X="-394.465588647342" Y="288.934262657717" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
        </ConnectivityNode>
        <ConnectivityNode pathName="SEMODELO/U/4U/grounded" name="grounded" desc="" />
        <ConnectivityNode pathName="SEMODELO/U/4U/4U-CON2" name="4U-CON2" desc="">
          <Private type="SchneiderElectric-SET-ConnectivityNode">
            <ConnectivityNodePrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" BusbarType="Point" />
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="30" Height="25" />
                    <Position X="-280.498838036991" Y="130.467428001956" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
        </ConnectivityNode>
        <ConnectivityNode pathName="SEMODELO/U/4U/CN0" name="CN0" />
        <ConnectivityNode pathName="SEMODELO/U/4U/CN2" name="CN2" />
      </Bay>
      <Bay name="BU1" desc="Barra 1">
        <Private type="SchneiderElectric-SET-UserInterface">
          <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
            <ElectricalWorkAreas>
              <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                  <Size Width="861.091820987654" Height="83.7522988060122" />
                  <Position X="-475.668692871094" Y="-400.112721435547" />
                </GraphicalProperties>
              </WorkArea>
            </ElectricalWorkAreas>
            <SystemWorkAreas />
            <LogicalWorkAreas />
          </MetaInfosPrivates>
        </Private>
        <Private type="SchneiderElectric-SET-IEDAssociations">
          <IEDAssociationPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
            <IEDsAssociatedCollection />
          </IEDAssociationPrivates>
        </Private>
        <ConnectivityNode pathName="SEMODELO/U/BU1/1BU" name="1BU" desc="">
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="805.73" Height="30" />
                    <Position X="-445.419458496094" Y="-361.878815185547" />
                    <RelativeLabelPosition X="-5" Y="-2.5" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-ConnectivityNode">
            <ConnectivityNodePrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" BusbarType="Horizontal" />
          </Private>
        </ConnectivityNode>
      </Bay>
      <Bay name="5U" desc="INTERLIGAÇÃO">
        <Private type="SchneiderElectric-SET-UserInterface">
          <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
            <ElectricalWorkAreas>
              <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                  <Size Width="378.453640407986" Height="594.228937092915" />
                  <Position X="-473.933993787968" Y="329.009221584765" />
                </GraphicalProperties>
              </WorkArea>
            </ElectricalWorkAreas>
            <SystemWorkAreas />
            <LogicalWorkAreas />
          </MetaInfosPrivates>
        </Private>
        <Private type="SchneiderElectric-SET-IEDAssociations">
          <IEDAssociationPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
            <IEDsAssociatedCollection>
              <IEDIdentification IEDName="RELE_BF_5U_P" IsAssociated="True" />
            </IEDsAssociatedCollection>
          </IEDAssociationPrivates>
        </Private>
        <LNode iedName="RELE_BF_5U_P" ldInst="LD0" prefix="PP_BF_" lnClass="RBRF" lnInst="1" lnType="RBRF_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <LNode iedName="RELE_BF_5U_P" ldInst="LD0" prefix="PP_BF94_" lnClass="PTRC" lnInst="1" lnType="PTRC_CEMIG">
          <Private type="SchneiderElectric-SET-LNode-SpecNaming">
            <LNodeSpecNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" LDName="" Prefix="null" Inst="null" />
          </Private>
        </LNode>
        <ConductingEquipment type="DIS" name="5U5" desc="">
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="None" />
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-427.523325554219" Y="797.24896778039" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode iedName="None" ldInst="" prefix="" lnClass="XSWI" lnInst="1" lnType="SE_XSWI_SET_default_V001" />
          <LNode iedName="None" ldInst="" prefix="" lnClass="CILO" lnInst="1" lnType="SE_CILO_SET_default_V001" />
          <Terminal name="TERMINAL1" connectivityNode="SEMODELO/U/6U/6U-CON1" substationName="SEMODELO" voltageLevelName="U" bayName="6U" cNodeName="6U-CON1">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/5U/CN1" substationName="SEMODELO" voltageLevelName="U" bayName="5U" cNodeName="CN1">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Top" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="CBR" name="5U4" desc="">
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="None" />
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-427.862680114219" Y="520.682775940391" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode iedName="None" ldInst="" prefix="" lnClass="XCBR" lnInst="1" lnType="SE_XCBR_SET_default_V001" />
          <LNode iedName="None" ldInst="" prefix="" lnClass="CSWI" lnInst="1" lnType="SE_CSWI_SET_default_V001" />
          <LNode iedName="None" ldInst="" prefix="CP_86B_" lnClass="CILO" lnInst="1" lnType="SE_CILO_SET_default_V001" />
          <LNode iedName="None" ldInst="" prefix="" lnClass="RBRF" lnInst="1" lnType="SE_RBRF_SET_default_V001" />
          <LNode iedName="None" ldInst="" prefix="" lnClass="RREC" lnInst="1" lnType="SE_RREC_SET_default_V001" />
          <LNode iedName="None" ldInst="" prefix="" lnClass="RSYN" lnInst="1" lnType="SE_RSYN_SET_default_V001" />
          <Terminal name="TERMINAL1" connectivityNode="SEMODELO/U/5U/CN3" substationName="SEMODELO" voltageLevelName="U" bayName="5U" cNodeName="CN3">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Top" />
            </Private>
          </Terminal>
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/5U/CN0" substationName="SEMODELO" voltageLevelName="U" bayName="5U" cNodeName="CN0">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="DIS" name="5U3" desc="">
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="None" />
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-428.329857394219" Y="388.924647780391" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode iedName="None" ldInst="" prefix="" lnClass="XSWI" lnInst="1" lnType="SE_XSWI_SET_default_V001" />
          <LNode iedName="None" ldInst="" prefix="" lnClass="CILO" lnInst="1" lnType="SE_CILO_SET_default_V001" />
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/5U/CN3" substationName="SEMODELO" voltageLevelName="U" bayName="5U" cNodeName="CN3">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
          <Terminal name="TERMINAL1" connectivityNode="SEMODELO/U/4U/4U-CON1" substationName="SEMODELO" voltageLevelName="U" bayName="4U" cNodeName="4U-CON1">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Top" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="CTR" name="5UTC" desc="">
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-427.650437158084" Y="662.055902123049" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="None" />
          </Private>
          <LNode prefix="" lnClass="TCTR" lnInst="1" lnType="SE_TCTR_SET_default_V001" />
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/5U/CN0" substationName="SEMODELO" voltageLevelName="U" bayName="5U" cNodeName="CN0">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Top" />
            </Private>
          </Terminal>
          <Terminal name="TERMINAL1" connectivityNode="SEMODELO/U/5U/CN1" substationName="SEMODELO" voltageLevelName="U" bayName="5U" cNodeName="CN1">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConnectivityNode pathName="SEMODELO/U/5U/CN3" name="CN3" />
        <ConnectivityNode pathName="SEMODELO/U/5U/CN0" name="CN0" />
        <ConnectivityNode pathName="SEMODELO/U/5U/CN1" name="CN1" />
      </Bay>
      <Bay name="6U" desc="LT COM REATOR">
        <Private type="SchneiderElectric-SET-UserInterface">
          <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
            <ElectricalWorkAreas>
              <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                  <Size Width="381.419949001736" Height="726.357843342915" />
                  <Position X="-474.763232069219" Y="933.381435940234" />
                </GraphicalProperties>
              </WorkArea>
            </ElectricalWorkAreas>
            <SystemWorkAreas />
            <LogicalWorkAreas />
          </MetaInfosPrivates>
        </Private>
        <Private type="SchneiderElectric-SET-IEDAssociations">
          <IEDAssociationPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
            <IEDsAssociatedCollection />
          </IEDAssociationPrivates>
        </Private>
        <ConductingEquipment type="CBR" name="6U4" desc="">
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="None" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-426.816918395469" Y="1372.44874029586" />
                    <RelativeLabelPosition X="-5" Y="32.5" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode iedName="None" ldInst="" prefix="" lnClass="XCBR" lnInst="1" lnType="SE_XCBR_SET_default_V001" />
          <LNode iedName="None" ldInst="" prefix="" lnClass="CSWI" lnInst="1" lnType="SE_CSWI_SET_default_V001" />
          <LNode iedName="None" ldInst="" prefix="" lnClass="CILO" lnInst="1" lnType="SE_CILO_SET_default_V001" />
          <LNode iedName="None" ldInst="" prefix="" lnClass="RBRF" lnInst="1" lnType="SE_RBRF_SET_default_V001" />
          <LNode iedName="None" ldInst="" prefix="" lnClass="RREC" lnInst="1" lnType="SE_RREC_SET_default_V001" />
          <LNode iedName="None" ldInst="" prefix="" lnClass="RSYN" lnInst="1" lnType="SE_RSYN_SET_default_V001" />
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/6U/CN0" substationName="SEMODELO" voltageLevelName="U" bayName="6U" cNodeName="CN0">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
          <Terminal name="TERMINAL1" connectivityNode="SEMODELO/U/6U/CN3" substationName="SEMODELO" voltageLevelName="U" bayName="6U" cNodeName="CN3">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Top" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="DIS" name="6U3" desc="">
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="None" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-426.596595675469" Y="1077.45311213586" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode iedName="None" ldInst="" prefix="" lnClass="XSWI" lnInst="1" lnType="SE_XSWI_SET_default_V001" />
          <LNode iedName="None" ldInst="" prefix="" lnClass="CILO" lnInst="1" lnType="SE_CILO_SET_default_V001" />
          <Terminal name="TERMINAL1" connectivityNode="SEMODELO/U/6U/6U-CON1" substationName="SEMODELO" voltageLevelName="U" bayName="6U" cNodeName="6U-CON1">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Top" />
            </Private>
          </Terminal>
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/6U/CN1" substationName="SEMODELO" voltageLevelName="U" bayName="6U" cNodeName="CN1">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="DIS" name="6U5" desc="">
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="None" />
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-427.132563835469" Y="1518.12243213586" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode iedName="None" ldInst="" prefix="" lnClass="XSWI" lnInst="1" lnType="SE_XSWI_SET_default_V001" />
          <LNode iedName="None" ldInst="" prefix="" lnClass="CILO" lnInst="1" lnType="SE_CILO_SET_default_V001" />
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/6U/CN0" substationName="SEMODELO" voltageLevelName="U" bayName="6U" cNodeName="CN0">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Top" />
            </Private>
          </Terminal>
          <Terminal name="TERMINAL1" connectivityNode="SEMODELO/U/BU2/2BU" substationName="SEMODELO" voltageLevelName="U" bayName="BU2" cNodeName="2BU">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="DIS" name="6U8" desc="">
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="Bottom" />
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-301.846389306522" Y="1071.83557741602" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode prefix="" lnClass="XSWI" lnInst="1" lnType="SE_XSWI_SET_default_V001" />
          <LNode prefix="" lnClass="CILO" lnInst="1" lnType="SE_CILO_SET_default_V001" />
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/6U/6U-CON1" substationName="SEMODELO" voltageLevelName="U" bayName="6U" cNodeName="6U-CON1">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Top" />
            </Private>
          </Terminal>
          <Terminal name="TERMINAL1" connectivityNode="SEMODELO/U/6U/6U-CON2" substationName="SEMODELO" voltageLevelName="U" bayName="6U" cNodeName="6U-CON2">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="DIS" name="6U8T" desc="">
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="Left" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-205.007094872928" Y="1211.91522829492" />
                    <RelativeLabelPosition X="-72.1875" Y="-33.125" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode prefix="" lnClass="XSWI" lnInst="1" lnType="SE_XSWI_SET_default_V001" />
          <LNode prefix="" lnClass="CILO" lnInst="1" lnType="SE_CILO_SET_default_V001" />
          <Terminal connectivityNode="SEMODELO/U/6U/grounded" substationName="SEMODELO" voltageLevelName="U" bayName="6U" cNodeName="grounded" />
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/6U/6U-CON2" substationName="SEMODELO" voltageLevelName="U" bayName="6U" cNodeName="6U-CON2">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Left" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="LIN" name="LT2" desc="">
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="None" />
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-301.882094872928" Y="1457.22772829492" />
                    <RelativeLabelPosition X="-65.9375" Y="109.0625" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode prefix="" lnClass="ZLIN" lnInst="1" lnType="SE_ZLIN_SET_default_V001" />
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/6U/6U-CON2" substationName="SEMODELO" voltageLevelName="U" bayName="6U" cNodeName="6U-CON2">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Top" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="CTR" name="6UTC" desc="">
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="None" />
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-425.699188378788" Y="1229.28267336328" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode prefix="" lnClass="TCTR" lnInst="1" lnType="SE_TCTR_SET_default_V001" />
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/6U/CN1" substationName="SEMODELO" voltageLevelName="U" bayName="6U" cNodeName="CN1">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Top" />
            </Private>
          </Terminal>
          <Terminal name="TERMINAL1" connectivityNode="SEMODELO/U/6U/CN3" substationName="SEMODELO" voltageLevelName="U" bayName="6U" cNodeName="CN3">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="VTR" name="6UTP" desc="">
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-204.196441224552" Y="1321.56837358911" />
                    <RelativeLabelPosition X="-76.875" Y="107.5" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="Left" />
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <LNode prefix="" lnClass="TVTR" lnInst="1" lnType="SE_TVTR_SET_default_V001" />
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/6U/6U-CON2" substationName="SEMODELO" voltageLevelName="U" bayName="6U" cNodeName="6U-CON2">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Left" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConnectivityNode pathName="SEMODELO/U/6U/CN0" name="CN0" />
        <ConnectivityNode pathName="SEMODELO/U/6U/6U-CON1" name="6U-CON1" desc="">
          <Private type="SchneiderElectric-SET-ConnectivityNode">
            <ConnectivityNodePrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" BusbarType="Point" />
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="30" Height="25" />
                    <Position X="-391.739119897342" Y="992.121067345217" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
        </ConnectivityNode>
        <ConnectivityNode pathName="SEMODELO/U/6U/grounded" name="grounded" desc="" />
        <ConnectivityNode pathName="SEMODELO/U/6U/6U-CON2" name="6U-CON2" desc="">
          <Private type="SchneiderElectric-SET-ConnectivityNode">
            <ConnectivityNodePrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" BusbarType="Point" />
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="30" Height="25" />
                    <Position X="-265.944594872928" Y="1249.41522829492" />
                    <RelativeLabelPosition X="-9.6875" Y="-6.5625" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
        </ConnectivityNode>
        <ConnectivityNode pathName="SEMODELO/U/6U/CN1" name="CN1" />
        <ConnectivityNode pathName="SEMODELO/U/6U/CN3" name="CN3" />
      </Bay>
      <Bay name="BU2" desc="Barra 2">
        <Private type="SchneiderElectric-SET-UserInterface">
          <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
            <ElectricalWorkAreas>
              <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                  <Size Width="861.091820987654" Height="83.7522988060122" />
                  <Position X="-479.584098144531" Y="1664.3770246582" />
                </GraphicalProperties>
              </WorkArea>
            </ElectricalWorkAreas>
            <SystemWorkAreas />
            <LogicalWorkAreas />
          </MetaInfosPrivates>
        </Private>
        <Private type="SchneiderElectric-SET-IEDAssociations">
          <IEDAssociationPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
            <IEDsAssociatedCollection />
          </IEDAssociationPrivates>
        </Private>
        <ConnectivityNode pathName="SEMODELO/U/BU2/2BU" name="2BU" desc="">
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-ConnectivityNode">
            <ConnectivityNodePrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" BusbarType="Horizontal" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="805.73" Height="30" />
                    <Position X="-442.010645019531" Y="1695.2867121582" />
                    <RelativeLabelPosition X="-5" Y="-2.5" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
        </ConnectivityNode>
      </Bay>
      <Bay name="6US" desc="REATOR EM LT">
        <Private type="SchneiderElectric-SET-UserInterface">
          <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
            <ElectricalWorkAreas>
              <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                  <Size Width="423.4375" Height="721.874999999999" />
                  <Position X="-90.6660762343176" Y="934.180769829346" />
                </GraphicalProperties>
              </WorkArea>
            </ElectricalWorkAreas>
            <SystemWorkAreas />
            <LogicalWorkAreas />
          </MetaInfosPrivates>
        </Private>
        <Private type="SchneiderElectric-SET-IEDAssociations">
          <IEDAssociationPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
            <IEDsAssociatedCollection />
          </IEDAssociationPrivates>
        </Private>
        <ConductingEquipment type="CTR" name="6USTC1" desc="">
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="None" />
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-14.495468286014" Y="1356.39921999414" />
                    <RelativeLabelPosition X="-5" Y="32.5" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode prefix="" lnClass="TCTR" lnInst="1" lnType="SE_TCTR_SET_default_V001" />
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/6US/CN4" substationName="SEMODELO" voltageLevelName="U" bayName="6US" cNodeName="CN4">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Top" />
            </Private>
          </Terminal>
          <Terminal name="TERMINAL1" connectivityNode="SEMODELO/U/6US/CN5" substationName="SEMODELO" voltageLevelName="U" bayName="6US" cNodeName="CN5">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="CBR" name="6US4" desc="">
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="None" />
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-14.3820948729283" Y="1210.35272829492" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode prefix="" lnClass="XCBR" lnInst="1" lnType="SE_XCBR_SET_default_V001" />
          <LNode prefix="" lnClass="CSWI" lnInst="1" lnType="SE_CSWI_SET_default_V001" />
          <LNode prefix="" lnClass="CILO" lnInst="1" lnType="SE_CILO_SET_default_V001" />
          <LNode prefix="" lnClass="RBRF" lnInst="1" lnType="SE_RBRF_SET_default_V001" />
          <LNode prefix="" lnClass="RREC" lnInst="1" lnType="SE_RREC_SET_default_V001" />
          <LNode prefix="" lnClass="RSYN" lnInst="1" lnType="SE_RSYN_SET_default_V001" />
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/6US/CN2" substationName="SEMODELO" voltageLevelName="U" bayName="6US" cNodeName="CN2">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Top" />
            </Private>
          </Terminal>
          <Terminal name="TERMINAL1" connectivityNode="SEMODELO/U/6US/CN4" substationName="SEMODELO" voltageLevelName="U" bayName="6US" cNodeName="CN4">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="DIS" name="6US3" desc="">
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="Bottom" />
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-12.8195948729283" Y="1021.29022829492" />
                    <RelativeLabelPosition X="4.375" Y="35.625" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode prefix="" lnClass="XSWI" lnInst="1" lnType="SE_XSWI_SET_default_V001" />
          <LNode prefix="" lnClass="CILO" lnInst="1" lnType="SE_CILO_SET_default_V001" />
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/6U/6U-CON1" substationName="SEMODELO" voltageLevelName="U" bayName="6U" cNodeName="6U-CON1">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Top" />
            </Private>
          </Terminal>
          <Terminal name="TERMINAL1" connectivityNode="SEMODELO/U/6US/CN2" substationName="SEMODELO" voltageLevelName="U" bayName="6US" cNodeName="CN2">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="EFN" name="TERRA1" desc="">
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="Bottom" />
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="124.210586401486" Y="1346.13066530664" />
                    <RelativeLabelPosition X="-73.75" Y="-37.8125" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode prefix="" lnClass="YEFN" lnInst="1" lnType="SE_YEFN_SET_default_V001" />
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/6US/CN7" substationName="SEMODELO" voltageLevelName="U" bayName="6US" cNodeName="CN7">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="CTR" name="6USTC2" desc="">
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="Bottom" />
          </Private>
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="128.398086401486" Y="1502.75566530664" />
                    <RelativeLabelPosition X="-162.1875" Y="29.0625" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode prefix="" lnClass="TCTR" lnInst="1" lnType="SE_TCTR_SET_default_V001" />
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/6US/CN6" substationName="SEMODELO" voltageLevelName="U" bayName="6US" cNodeName="CN6">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
          <Terminal name="TERMINAL1" connectivityNode="SEMODELO/U/6US/CN7" substationName="SEMODELO" voltageLevelName="U" bayName="6US" cNodeName="CN7">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Top" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConductingEquipment type="REA" name="S1" desc="">
          <Private type="SchneiderElectric-SET-AutoNaming">
            <AutoNamingPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsEnabled="False" />
          </Private>
          <Private type="SchneiderElectric-SET-PrimaryEquipment">
            <PrimaryEquipmentPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsParentLocked="False" Orientation="None" />
          </Private>
          <Private type="SchneiderElectric-SET-UserInterface">
            <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
              <ElectricalWorkAreas>
                <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
                  <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                    <Size Width="100" Height="100" />
                    <Position X="-12.8195948729283" Y="1496.29022829492" />
                  </GraphicalProperties>
                </WorkArea>
              </ElectricalWorkAreas>
              <SystemWorkAreas />
              <LogicalWorkAreas />
            </MetaInfosPrivates>
          </Private>
          <LNode prefix="" lnClass="ZREA" lnInst="1" lnType="SE_ZREA_SET_default_V001" />
          <Terminal name="TERMINAL0" connectivityNode="SEMODELO/U/6US/CN5" substationName="SEMODELO" voltageLevelName="U" bayName="6US" cNodeName="CN5">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Top" />
            </Private>
          </Terminal>
          <Terminal name="TERMINAL1" connectivityNode="SEMODELO/U/6US/CN6" substationName="SEMODELO" voltageLevelName="U" bayName="6US" cNodeName="CN6">
            <Private type="SchneiderElectric-SET-Terminal">
              <TerminalPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" TerminalPosition="Bottom" />
            </Private>
          </Terminal>
        </ConductingEquipment>
        <ConnectivityNode pathName="SEMODELO/U/6US/CN7" name="CN7" />
        <ConnectivityNode pathName="SEMODELO/U/6US/CN6" name="CN6" />
        <ConnectivityNode pathName="SEMODELO/U/6US/CN5" name="CN5" />
        <ConnectivityNode pathName="SEMODELO/U/6US/CN4" name="CN4" />
        <ConnectivityNode pathName="SEMODELO/U/6US/CN2" name="CN2" />
      </Bay>
    </VoltageLevel>
  </Substation>
  <Communication>
    <SubNetwork type="8-MMS" name="SE_MODELO" desc="">
      <Private type="SchneiderElectric-SET-SBusStructureNetworkData">
        <SBusStructureNetworkDataPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" Redundancy="True" NetworkTopology="Ring" Media="Electric" />
      </Private>
      <Private type="SchneiderElectric-SET-SubnetworkIPV4Plan">
        <SubNetworkIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" Deployed="Yes">
          <IPAddress IsWarning="False" IsError="None">
            <Octet_1 PartValue="190" IsLocal="True" />
            <Octet_2 PartValue="168" IsLocal="False" />
            <Octet_3 PartValue="0:255" IsLocal="False" />
            <Octet_4 PartValue="1:255" IsLocal="False" />
          </IPAddress>
          <Subnetmask IsWarning="False" IsError="None">
            <Octet_1 PartValue="255" IsLocal="False" />
            <Octet_2 PartValue="255" IsLocal="False" />
            <Octet_3 PartValue="255" IsLocal="False" />
            <Octet_4 PartValue="0" IsLocal="False" />
          </Subnetmask>
          <Gateway IsWarning="False" IsError="None">
            <Octet_1 PartValue="0" IsLocal="False" />
            <Octet_2 PartValue="0" IsLocal="False" />
            <Octet_3 PartValue="0" IsLocal="False" />
            <Octet_4 PartValue="0" IsLocal="False" />
          </Gateway>
        </SubNetworkIPV4PlanPrivates>
      </Private>
      <Private type="SchneiderElectric-SET-UserInterface">
        <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
          <ElectricalWorkAreas />
          <SystemWorkAreas>
            <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000002">
              <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                <Size Width="107.426666666667" Height="38" />
                <Position X="217" Y="316" />
              </GraphicalProperties>
            </WorkArea>
          </SystemWorkAreas>
          <LogicalWorkAreas />
        </MetaInfosPrivates>
      </Private>
      <ConnectedAP iedName="U_CENTRAL" apName="AP1">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">190.168.0.3</P>
          <P type="IP-SUBNET">255.255.255.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
      </ConnectedAP>
      <ConnectedAP iedName="ICE_ST_4U" apName="AP1_ICE_1">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">190.168.0.1</P>
          <P type="IP-SUBNET">255.255.255.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
      </ConnectedAP>
      <ConnectedAP iedName="ICE_5U" apName="AP1_ICE_2">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">190.168.0.2</P>
          <P type="IP-SUBNET">255.255.255.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
      </ConnectedAP>
      <ConnectedAP iedName="ICE_2" apName="AP2_ICE_2">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">190.168.0.4</P>
          <P type="IP-SUBNET">255.255.255.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
      </ConnectedAP>
    </SubNetwork>
    <SubNetwork type="8-MMS" name="ST_BUS_4U" desc="">
      <Private type="SchneiderElectric-SET-SubnetworkIPV4Plan">
        <SubNetworkIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" Deployed="Yes">
          <IPAddress IsWarning="False" IsError="None">
            <Octet_1 PartValue="192" IsLocal="False" />
            <Octet_2 PartValue="168" IsLocal="False" />
            <Octet_3 PartValue="0:255" IsLocal="False" />
            <Octet_4 PartValue="1:255" IsLocal="False" />
          </IPAddress>
          <Subnetmask IsWarning="False" IsError="None">
            <Octet_1 PartValue="255" IsLocal="False" />
            <Octet_2 PartValue="255" IsLocal="False" />
            <Octet_3 PartValue="255" IsLocal="False" />
            <Octet_4 PartValue="0" IsLocal="False" />
          </Subnetmask>
          <Gateway IsWarning="False" IsError="None">
            <Octet_1 PartValue="0" IsLocal="False" />
            <Octet_2 PartValue="0" IsLocal="False" />
            <Octet_3 PartValue="0" IsLocal="False" />
            <Octet_4 PartValue="0" IsLocal="False" />
          </Gateway>
        </SubNetworkIPV4PlanPrivates>
      </Private>
      <Private type="SchneiderElectric-SET-UserInterface">
        <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
          <ElectricalWorkAreas />
          <SystemWorkAreas>
            <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000002">
              <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                <Size Width="97.83" Height="36" />
                <Position X="287.525" Y="512.5" />
              </GraphicalProperties>
            </WorkArea>
          </SystemWorkAreas>
          <LogicalWorkAreas />
        </MetaInfosPrivates>
      </Private>
      <Private type="SchneiderElectric-SET-SBusStructureNetworkData">
        <SBusStructureNetworkDataPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" Redundancy="True" NetworkTopology="Star" Media="Optic" />
      </Private>
      <ConnectedAP iedName="ICE_ST_4U" apName="AP2_ICE_1">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">192.168.0.1</P>
          <P type="IP-SUBNET">255.255.254.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
      </ConnectedAP>
      <ConnectedAP iedName="UAC_4U_P" apName="AP1">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">192.168.0.11</P>
          <P type="IP-SUBNET">255.255.254.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
        <GSE ldInst="LD0" cbName="SETGooseCB_1">
          <Address>
            <P type="MAC-Address">00-00-00-00-00-00</P>
            <P type="APPID">0008</P>
            <P type="VLAN-PRIORITY">0</P>
            <P type="VLAN-ID">000</P>
          </Address>
        </GSE>
      </ConnectedAP>
      <ConnectedAP iedName="RELE_BF_4U_S" apName="AP1">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">192.168.0.8</P>
          <P type="IP-SUBNET">255.255.254.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
      </ConnectedAP>
      <ConnectedAP iedName="RELE_BF_4U_P" apName="AP1">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">192.168.0.7</P>
          <P type="IP-SUBNET">255.255.254.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
        <GSE ldInst="LD0" cbName="SETGooseCB_1">
          <Address>
            <P type="MAC-Address">00-00-00-00-00-00</P>
            <P type="APPID">0000</P>
            <P type="VLAN-PRIORITY">0</P>
            <P type="VLAN-ID">000</P>
          </Address>
        </GSE>
      </ConnectedAP>
      <ConnectedAP iedName="RELE_S_LT1" apName="AP1">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">192.168.0.10</P>
          <P type="IP-SUBNET">255.255.254.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
        <GSE ldInst="LD0" cbName="SETGooseCB_1">
          <Address>
            <P type="MAC-Address">00-00-00-00-00-00</P>
            <P type="APPID">0007</P>
            <P type="VLAN-PRIORITY">0</P>
            <P type="VLAN-ID">000</P>
          </Address>
        </GSE>
      </ConnectedAP>
      <ConnectedAP iedName="RELE_P_LT1" apName="AP1">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">192.168.0.9</P>
          <P type="IP-SUBNET">255.255.254.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
        <GSE ldInst="LD0" cbName="SETGooseCB_1">
          <Address>
            <P type="MAC-Address">00-00-00-00-00-00</P>
            <P type="APPID">0006</P>
            <P type="VLAN-PRIORITY">0</P>
            <P type="VLAN-ID">000</P>
          </Address>
        </GSE>
      </ConnectedAP>
      <ConnectedAP iedName="UAC_4U_S" apName="AP1">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">192.168.0.12</P>
          <P type="IP-SUBNET">255.255.254.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
        <GSE ldInst="LD0" cbName="SETGooseCB_1">
          <Address>
            <P type="MAC-Address">00-00-00-00-00-00</P>
            <P type="APPID">0009</P>
            <P type="VLAN-PRIORITY">0</P>
            <P type="VLAN-ID">000</P>
          </Address>
        </GSE>
      </ConnectedAP>
      <ConnectedAP iedName="RDP_4U" apName="AP1">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">192.168.0.6</P>
          <P type="IP-SUBNET">255.255.254.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
      </ConnectedAP>
      <ConnectedAP iedName="ICE_1" apName="AP2_ICE_1">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">192.168.0.15</P>
          <P type="IP-SUBNET">255.255.254.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
      </ConnectedAP>
      <ConnectedAP iedName="ICE_3" apName="AP1_ICE_3">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">192.168.0.14</P>
          <P type="IP-SUBNET">255.255.255.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
      </ConnectedAP>
    </SubNetwork>
    <SubNetwork type="8-MMS" name="PROC_BUS_4U" desc="">
      <Private type="SchneiderElectric-SET-SBusStructureNetworkData">
        <SBusStructureNetworkDataPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" Redundancy="False" NetworkTopology="None" Media="None" />
      </Private>
      <Private type="SchneiderElectric-SET-UserInterface">
        <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
          <ElectricalWorkAreas />
          <SystemWorkAreas>
            <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000002">
              <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                <Size Width="115.826666666667" Height="36" />
                <Position X="408.25" Y="616.212625" />
              </GraphicalProperties>
            </WorkArea>
          </SystemWorkAreas>
          <LogicalWorkAreas />
        </MetaInfosPrivates>
      </Private>
      <Private type="SchneiderElectric-SET-SubnetworkIPV4Plan">
        <SubNetworkIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" Deployed="Yes">
          <IPAddress IsWarning="False" IsError="None">
            <Octet_1 PartValue="192" IsLocal="False" />
            <Octet_2 PartValue="168" IsLocal="False" />
            <Octet_3 PartValue="0:255" IsLocal="False" />
            <Octet_4 PartValue="1:255" IsLocal="False" />
          </IPAddress>
          <Subnetmask IsWarning="False" IsError="None">
            <Octet_1 PartValue="255" IsLocal="False" />
            <Octet_2 PartValue="255" IsLocal="False" />
            <Octet_3 PartValue="255" IsLocal="False" />
            <Octet_4 PartValue="0" IsLocal="False" />
          </Subnetmask>
          <Gateway IsWarning="False" IsError="None">
            <Octet_1 PartValue="0" IsLocal="False" />
            <Octet_2 PartValue="0" IsLocal="False" />
            <Octet_3 PartValue="0" IsLocal="False" />
            <Octet_4 PartValue="0" IsLocal="False" />
          </Gateway>
        </SubNetworkIPV4PlanPrivates>
      </Private>
      <ConnectedAP iedName="ICE_1" apName="AP1_ICE_1">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">192.168.0.1</P>
          <P type="IP-SUBNET">255.255.255.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
      </ConnectedAP>
      <ConnectedAP iedName="ICE_2" apName="AP1_ICE_2">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">192.168.0.2</P>
          <P type="IP-SUBNET">255.255.255.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
      </ConnectedAP>
      <ConnectedAP iedName="PATIO_4U3" apName="AP1">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">192.168.0.3</P>
          <P type="IP-SUBNET">255.255.255.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
        <GSE ldInst="LD0" cbName="SETGooseCB_1">
          <Address>
            <P type="MAC-Address">00-00-00-00-00-00</P>
            <P type="APPID">0002</P>
            <P type="VLAN-PRIORITY">0</P>
            <P type="VLAN-ID">000</P>
          </Address>
        </GSE>
      </ConnectedAP>
      <ConnectedAP iedName="PATIO_4U4" apName="AP1">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">192.168.0.4</P>
          <P type="IP-SUBNET">255.255.255.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
        <GSE ldInst="LD0" cbName="SETGooseCB_1">
          <Address>
            <P type="MAC-Address">00-00-00-00-00-00</P>
            <P type="APPID">0005</P>
            <P type="VLAN-PRIORITY">0</P>
            <P type="VLAN-ID">000</P>
          </Address>
        </GSE>
      </ConnectedAP>
      <ConnectedAP iedName="PATIO_4U5" apName="AP1">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">192.168.0.5</P>
          <P type="IP-SUBNET">255.255.255.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
        <GSE ldInst="LD0" cbName="SETGooseCB_1">
          <Address>
            <P type="MAC-Address">00-00-00-00-00-00</P>
            <P type="APPID">000A</P>
            <P type="VLAN-PRIORITY">0</P>
            <P type="VLAN-ID">000</P>
          </Address>
        </GSE>
      </ConnectedAP>
      <ConnectedAP iedName="PATIO_4U8" apName="AP1">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">192.168.0.13</P>
          <P type="IP-SUBNET">255.255.255.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
        <GSE ldInst="LD0" cbName="SETGooseCB_1">
          <Address>
            <P type="MAC-Address">00-00-00-00-00-00</P>
            <P type="APPID">000B</P>
            <P type="VLAN-PRIORITY">0</P>
            <P type="VLAN-ID">000</P>
          </Address>
        </GSE>
      </ConnectedAP>
    </SubNetwork>
    <SubNetwork type="8-MMS" name="ST_BUS_5U" desc="">
      <Private type="SchneiderElectric-SET-SubnetworkIPV4Plan">
        <SubNetworkIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" Deployed="Yes">
          <IPAddress IsWarning="False" IsError="None">
            <Octet_1 PartValue="192" IsLocal="False" />
            <Octet_2 PartValue="168" IsLocal="False" />
            <Octet_3 PartValue="0:255" IsLocal="False" />
            <Octet_4 PartValue="1:255" IsLocal="False" />
          </IPAddress>
          <Subnetmask IsWarning="False" IsError="None">
            <Octet_1 PartValue="255" IsLocal="False" />
            <Octet_2 PartValue="255" IsLocal="False" />
            <Octet_3 PartValue="255" IsLocal="False" />
            <Octet_4 PartValue="0" IsLocal="False" />
          </Subnetmask>
          <Gateway IsWarning="False" IsError="None">
            <Octet_1 PartValue="0" IsLocal="False" />
            <Octet_2 PartValue="0" IsLocal="False" />
            <Octet_3 PartValue="0" IsLocal="False" />
            <Octet_4 PartValue="0" IsLocal="False" />
          </Gateway>
        </SubNetworkIPV4PlanPrivates>
      </Private>
      <Private type="SchneiderElectric-SET-UserInterface">
        <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
          <ElectricalWorkAreas />
          <SystemWorkAreas>
            <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000002">
              <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
                <Size Width="97.83" Height="36" />
                <Position X="66" Y="513.5" />
              </GraphicalProperties>
            </WorkArea>
          </SystemWorkAreas>
          <LogicalWorkAreas />
        </MetaInfosPrivates>
      </Private>
      <Private type="SchneiderElectric-SET-SBusStructureNetworkData">
        <SBusStructureNetworkDataPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" Redundancy="True" NetworkTopology="Star" Media="None" />
      </Private>
      <ConnectedAP iedName="ICE_5U" apName="AP2_ICE_2">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">192.168.0.1</P>
          <P type="IP-SUBNET">255.255.255.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
      </ConnectedAP>
      <ConnectedAP iedName="RELE_BF_5U_P" apName="AP1">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">192.168.0.2</P>
          <P type="IP-SUBNET">255.255.255.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
        <GSE ldInst="LD0" cbName="SETGooseCB_1">
          <Address>
            <P type="MAC-Address">00-00-00-00-00-00</P>
            <P type="APPID">000C</P>
            <P type="VLAN-PRIORITY">0</P>
            <P type="VLAN-ID">000</P>
          </Address>
        </GSE>
      </ConnectedAP>
      <ConnectedAP iedName="ICE_3" apName="AP2_ICE_3">
        <Private type="SchneiderElectric-SET-ConnectedApIPV4Plan">
          <ConnectedApIPV4PlanPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" IsLocked="False" IsEditedToDeploy="False" />
        </Private>
        <Address>
          <P type="IP">192.168.0.16</P>
          <P type="IP-SUBNET">255.255.255.0</P>
          <P type="IP-GATEWAY">0.0.0.0</P>
        </Address>
      </ConnectedAP>
    </SubNetwork>
  </Communication>
  <IED name="ICE_1" desc="">
    <Private type="SchneiderElectric-SET-UserInterface">
      <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <ElectricalWorkAreas />
        <SystemWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000002">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="345.54375" Y="564.3563125" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="5570cec2-94b6-41fa-bf42-9026ab8f5b26">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="50" Height="50" />
              <Position X="50" Y="50" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="05654ba5-3902-482b-9432-5fce4e2c1635">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="50" Height="50" />
              <Position X="125" Y="50" />
            </GraphicalProperties>
          </WorkArea>
        </SystemWorkAreas>
        <LogicalWorkAreas />
      </MetaInfosPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-IEDProperties">
      <IEDPropertiesPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" ICDType="" IIDCIDType="" IEDClass="" ICDRevision="-1" SetRevision="0" IsInError="False" ElementType="ICE" ElementFunction="None" />
    </Private>
    <Private type="SchneiderElectric-SET-InterConnectedEquipment">
      <ICEPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" GooseFlow="True" ReportFlow="True" />
    </Private>
    <Private type="SchneiderElectric-SET-IEDSubscribeEditorFilter">
      <IEDSubscribeEditorFilterPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" MappedLNChecked="False" PointToPointChecked="False" BufferMXChecked="False" BufferSTChecked="False" IsAutomaticQualityClient="False" />
    </Private>
    <AccessPoint name="AP1_ICE_1" clock="false" />
    <AccessPoint name="AP2_ICE_1" />
  </IED>
  <IED name="ICE_5U" desc="">
    <Private type="SchneiderElectric-SET-IEDSubscribeEditorFilter">
      <IEDSubscribeEditorFilterPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" MappedLNChecked="False" PointToPointChecked="False" BufferMXChecked="False" BufferSTChecked="False" IsAutomaticQualityClient="False" />
    </Private>
    <Private type="SchneiderElectric-SET-IEDProperties">
      <IEDPropertiesPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" ICDType="" IIDCIDType="" IEDClass="" ICDRevision="-1" SetRevision="0" IsInError="False" ElementType="ICE" ElementFunction="None" />
    </Private>
    <Private type="SchneiderElectric-SET-InterConnectedEquipment">
      <ICEPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" GooseFlow="True" ReportFlow="True" />
    </Private>
    <Private type="SchneiderElectric-SET-UserInterface">
      <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <ElectricalWorkAreas />
        <SystemWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000002">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="50" Height="50" />
              <Position X="141.5" Y="414.75" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="da9b4ce3-826f-4bd4-a5cc-55623ea380fc">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="50" Height="50" />
              <Position X="188" Y="233" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="3c25b338-3eed-4c5a-98f7-99547bf9429f">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="50" Height="50" />
              <Position X="50" Y="50" />
            </GraphicalProperties>
          </WorkArea>
        </SystemWorkAreas>
        <LogicalWorkAreas />
      </MetaInfosPrivates>
    </Private>
    <AccessPoint name="AP1_ICE_2" clock="false" />
    <AccessPoint name="AP2_ICE_2" />
  </IED>
  <IED name="RELE_P_LT1" desc="">
    <Private type="SchneiderElectric-SET-ConsumerOrder">
      <ConsumerOrderPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <IEDNameOrder>
          <Item>
            <Key>
              <String>PATIO_4U4</String>
            </Key>
            <Value>
              <Int32>0</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>RDP_4U</String>
            </Key>
            <Value>
              <Int32>1</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>UAC_4U_P</String>
            </Key>
            <Value>
              <Int32>2</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>UAC_4U_S</String>
            </Key>
            <Value>
              <Int32>3</Int32>
            </Value>
          </Item>
        </IEDNameOrder>
      </ConsumerOrderPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-UserInterface">
      <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <ElectricalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="0" Height="0" />
              <Position X="0" Y="0" />
            </GraphicalProperties>
          </WorkArea>
        </ElectricalWorkAreas>
        <SystemWorkAreas>
          <WorkArea WorkAreaIdentification="da9b4ce3-826f-4bd4-a5cc-55623ea380fc">
            <GraphicalProperties DuplicateCount="1" IsPlaced="False" IsExpanded="False">
              <Size Width="50" Height="50" />
              <Position X="350" Y="50" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="05654ba5-3902-482b-9432-5fce4e2c1635">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="174" Y="185" />
            </GraphicalProperties>
          </WorkArea>
        </SystemWorkAreas>
        <LogicalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000003">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="True">
              <Size Width="135.053333333333" Height="40" />
              <Position X="591.75" Y="394.6875" />
            </GraphicalProperties>
          </WorkArea>
        </LogicalWorkAreas>
      </MetaInfosPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-IEDProperties">
      <IEDPropertiesPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" ICDType="" IIDCIDType="" IEDClass="" ICDRevision="-1" SetRevision="0" IsInError="False" ElementType="IED" ElementFunction="None" />
    </Private>
    <Private type="SchneiderElectric-SET-IEDSubscribeEditorFilter">
      <IEDSubscribeEditorFilterPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" MappedLNChecked="False" PointToPointChecked="False" BufferMXChecked="False" BufferSTChecked="False" IsAutomaticQualityClient="False" />
    </Private>
    <Services>
      <ConfDataSet modify="true" maxAttributes="100" max="100" />
      <ConfReportControl bufMode="both" max="64" />
      <GOOSE max="64" />
      <ConfLNs fixPrefix="false" fixLnInst="false" />
      <ConfLdName />
    </Services>
    <AccessPoint name="AP1" clock="false">
      <Server>
        <Authentication />
        <LDevice inst="LD0">
          <LN0 lnClass="LLN0" inst="" lnType="SE_LLN0_SET_default_V001">
            <DataSet name="SETGooseDS_1" desc="">
              <FCDA ldInst="LD0" prefix="PP_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="phsA" fc="ST" />
              <FCDA ldInst="LD0" prefix="PP_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="phsB" fc="ST" />
              <FCDA ldInst="LD0" prefix="PP_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="phsC" fc="ST" />
              <FCDA ldInst="LD0" prefix="PP_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PP_Z1_" lnClass="PDIS" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PP_Z2_" lnClass="PDIS" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PP_Z3_" lnClass="PDIS" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PP_ZP_" lnClass="PDIS" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PP_67N_" lnClass="PTOC" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PP_59_" lnClass="PTOV" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PP_79_" lnClass="RREC" lnInst="1" doName="OpCls" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PP_78_" lnClass="PPAM" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PP_68_" lnClass="RPSB" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PP_SOTF_" lnClass="PSOF" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PP_TLP_" lnClass="PSCH" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PP_TLP_" lnClass="PSCH" lnInst="1" doName="RxTr1" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PP_79_5U4" lnClass="RREC" lnInst="1" doName="OpCls" daName="general" fc="ST" />
            </DataSet>
            <GSEControl type="GOOSE" appID="RELE_P_LT1LD0/LLN0$GO$SETGooseCB_1" confRev="30000" name="SETGooseCB_1" datSet="SETGooseDS_1">
              <IEDName>PATIO_4U4</IEDName>
              <IEDName>RDP_4U</IEDName>
              <IEDName>UAC_4U_P</IEDName>
              <IEDName>UAC_4U_S</IEDName>
            </GSEControl>
          </LN0>
          <LN prefix="PP_Z1_" lnClass="PDIS" inst="1" lnType="PDIS_CEMIG" />
          <LN prefix="PP_Z2_" lnClass="PDIS" inst="1" lnType="PDIS_CEMIG" />
          <LN prefix="PP_Z3_" lnClass="PDIS" inst="1" lnType="PDIS_CEMIG" />
          <LN prefix="PP_ZP_" lnClass="PDIS" inst="1" lnType="PDIS_CEMIG" />
          <LN prefix="PP_79_" lnClass="RREC" inst="1" lnType="RREC_CEMIG">
            <Inputs>
              <ExtRef desc="" iedName="PATIO_4U4" ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U4" ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Health" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U4" ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="CBOpCap" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="UAC_4U_P" ldInst="LD0" prefix="CP_86B_" lnClass="CILO" lnInst="1" doName="EnaCls" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="UAC_4U_P" ldInst="LD0" prefix="CP_86BF_" lnClass="CILO" lnInst="1" doName="EnaCls" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="UAC_4U_P" ldInst="LD0" prefix="CP_86TT_" lnClass="CILO" lnInst="1" doName="EnaCls" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="UAC_4U_S" ldInst="LD0" prefix="CS_86B_" lnClass="CILO" lnInst="1" doName="EnaCls" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="UAC_4U_S" ldInst="LD0" prefix="CS_86BF_" lnClass="CILO" lnInst="1" doName="EnaCls" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="UAC_4U_S" ldInst="LD0" prefix="CS_86TT_" lnClass="CILO" lnInst="1" doName="EnaCls" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
          <LN prefix="PP_50_" lnClass="PIOC" inst="1" lnType="PIOC_CEMIG" />
          <LN prefix="PP_51_" lnClass="PTOC" inst="1" lnType="PTOC_CEMIG" />
          <LN prefix="PP_59_" lnClass="PTOV" inst="1" lnType="PTOV_CEMIG" />
          <LN prefix="PP_67N_" lnClass="PTOC" inst="1" lnType="PTOC_CEMIG" />
          <LN prefix="PP_68_" lnClass="RPSB" inst="1" lnType="RPSB_CEMIG" />
          <LN prefix="PP_78_" lnClass="PPAM" inst="1" lnType="PPAM_CEMIG" />
          <LN prefix="PP_94_" lnClass="PTRC" inst="1" lnType="PTRC_CEMIG" />
          <LN prefix="PP_SOTF_" lnClass="PSOF" inst="1" lnType="PSOF_CEMIG" />
          <LN prefix="PP_TLP_" lnClass="PSCH" inst="1" lnType="PSCH_CEMIG" />
          <LN prefix="PP_79_5U4" lnClass="RREC" inst="1" lnType="RREC_CEMIG" />
        </LDevice>
      </Server>
    </AccessPoint>
  </IED>
  <IED name="RELE_BF_4U_P" desc="">
    <Private type="SchneiderElectric-SET-ConsumerOrder">
      <ConsumerOrderPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <IEDNameOrder>
          <Item>
            <Key>
              <String>PATIO_4U4</String>
            </Key>
            <Value>
              <Int32>0</Int32>
            </Value>
          </Item>
        </IEDNameOrder>
      </ConsumerOrderPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-UserInterface">
      <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <ElectricalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="0" Height="0" />
              <Position X="0" Y="0" />
            </GraphicalProperties>
          </WorkArea>
        </ElectricalWorkAreas>
        <SystemWorkAreas>
          <WorkArea WorkAreaIdentification="da9b4ce3-826f-4bd4-a5cc-55623ea380fc">
            <GraphicalProperties DuplicateCount="1" IsPlaced="False" IsExpanded="False">
              <Size Width="50" Height="50" />
              <Position X="425" Y="50" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="05654ba5-3902-482b-9432-5fce4e2c1635">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="283" Y="183" />
            </GraphicalProperties>
          </WorkArea>
        </SystemWorkAreas>
        <LogicalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000003">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="True">
              <Size Width="149.08" Height="40" />
              <Position X="1113.234375" Y="908.65625" />
            </GraphicalProperties>
          </WorkArea>
        </LogicalWorkAreas>
      </MetaInfosPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-IEDProperties">
      <IEDPropertiesPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" ICDType="" IIDCIDType="" IEDClass="" ICDRevision="-1" SetRevision="0" IsInError="False" ElementType="IED" ElementFunction="None" />
    </Private>
    <Private type="SchneiderElectric-SET-IEDSubscribeEditorFilter">
      <IEDSubscribeEditorFilterPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" MappedLNChecked="False" PointToPointChecked="False" BufferMXChecked="False" BufferSTChecked="False" IsAutomaticQualityClient="False" />
    </Private>
    <Services>
      <ConfDataSet modify="true" maxAttributes="100" max="100" />
      <ConfReportControl bufMode="both" max="64" />
      <GOOSE max="64" />
      <ConfLNs fixPrefix="false" fixLnInst="false" />
      <ConfLdName />
    </Services>
    <AccessPoint name="AP1" clock="false">
      <Server>
        <Authentication />
        <LDevice inst="LD0">
          <LN0 lnClass="LLN0" inst="" lnType="SE_LLN0_SET_default_V001">
            <GSEControl type="GOOSE" appID="RELE_BF_4U_PLD0/LLN0$GO$SETGooseCB_1" confRev="0" name="SETGooseCB_1">
              <IEDName>PATIO_4U4</IEDName>
            </GSEControl>
          </LN0>
          <LN prefix="PP_BF_" lnClass="RBRF" inst="1" lnType="RBRF_CEMIG">
            <Inputs>
              <ExtRef desc="" iedName="RELE_BF_5U_P" ldInst="LD0" prefix="PP_BF94_" lnClass="PTRC" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U4" ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
          <LN prefix="PP_BF94_" lnClass="PTRC" inst="1" lnType="PTRC_CEMIG" />
        </LDevice>
      </Server>
    </AccessPoint>
  </IED>
  <IED name="ICE_ST_4U" desc="">
    <Private type="SchneiderElectric-SET-IEDSubscribeEditorFilter">
      <IEDSubscribeEditorFilterPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" MappedLNChecked="False" PointToPointChecked="False" BufferMXChecked="False" BufferSTChecked="False" IsAutomaticQualityClient="False" />
    </Private>
    <Private type="SchneiderElectric-SET-UserInterface">
      <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <ElectricalWorkAreas />
        <SystemWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000002">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="50" Height="50" />
              <Position X="295.2" Y="408.05" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="05654ba5-3902-482b-9432-5fce4e2c1635">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="50" Height="50" />
              <Position X="51" Y="50" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="da9b4ce3-826f-4bd4-a5cc-55623ea380fc">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="50" Height="50" />
              <Position X="575" Y="50" />
            </GraphicalProperties>
          </WorkArea>
        </SystemWorkAreas>
        <LogicalWorkAreas />
      </MetaInfosPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-IEDProperties">
      <IEDPropertiesPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" ICDType="" IIDCIDType="" IEDClass="" ICDRevision="-1" SetRevision="0" IsInError="False" ElementType="ICE" ElementFunction="None" />
    </Private>
    <Private type="SchneiderElectric-SET-InterConnectedEquipment">
      <ICEPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" GooseFlow="True" ReportFlow="True" />
    </Private>
    <AccessPoint name="AP1_ICE_1" clock="false" />
    <AccessPoint name="AP2_ICE_1" />
  </IED>
  <IED name="RELE_S_LT1" desc="">
    <Private type="SchneiderElectric-SET-UserInterface">
      <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <ElectricalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="0" Height="0" />
              <Position X="0" Y="0" />
            </GraphicalProperties>
          </WorkArea>
        </ElectricalWorkAreas>
        <SystemWorkAreas>
          <WorkArea WorkAreaIdentification="da9b4ce3-826f-4bd4-a5cc-55623ea380fc">
            <GraphicalProperties DuplicateCount="1" IsPlaced="False" IsExpanded="False">
              <Size Width="50" Height="50" />
              <Position X="500" Y="50" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="05654ba5-3902-482b-9432-5fce4e2c1635">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="174" Y="222" />
            </GraphicalProperties>
          </WorkArea>
        </SystemWorkAreas>
        <LogicalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000003">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="True">
              <Size Width="134.706666666667" Height="40" />
              <Position X="745.265625" Y="393.09375" />
            </GraphicalProperties>
          </WorkArea>
        </LogicalWorkAreas>
      </MetaInfosPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-ConsumerOrder">
      <ConsumerOrderPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <IEDNameOrder>
          <Item>
            <Key>
              <String>PATIO_4U4</String>
            </Key>
            <Value>
              <Int32>0</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>RDP_4U</String>
            </Key>
            <Value>
              <Int32>1</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>UAC_4U_P</String>
            </Key>
            <Value>
              <Int32>2</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>UAC_4U_S</String>
            </Key>
            <Value>
              <Int32>3</Int32>
            </Value>
          </Item>
        </IEDNameOrder>
      </ConsumerOrderPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-IEDProperties">
      <IEDPropertiesPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" ICDType="" IIDCIDType="" IEDClass="" ICDRevision="-1" SetRevision="0" IsInError="False" ElementType="IED" ElementFunction="None" />
    </Private>
    <Private type="SchneiderElectric-SET-IEDSubscribeEditorFilter">
      <IEDSubscribeEditorFilterPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" MappedLNChecked="False" PointToPointChecked="False" BufferMXChecked="False" BufferSTChecked="False" IsAutomaticQualityClient="False" />
    </Private>
    <Services>
      <ConfDataSet modify="true" maxAttributes="100" max="100" />
      <ConfReportControl bufMode="both" max="64" />
      <GOOSE max="64" />
      <ConfLNs fixPrefix="false" fixLnInst="false" />
      <ConfLdName />
    </Services>
    <AccessPoint name="AP1" clock="false">
      <Server>
        <Authentication />
        <LDevice inst="LD0">
          <LN0 lnClass="LLN0" inst="" lnType="SE_LLN0_SET_default_V001">
            <DataSet name="SETGooseDS_1" desc="">
              <FCDA ldInst="LD0" prefix="PS_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="phsA" fc="ST" />
              <FCDA ldInst="LD0" prefix="PS_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="phsB" fc="ST" />
              <FCDA ldInst="LD0" prefix="PS_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="phsC" fc="ST" />
              <FCDA ldInst="LD0" prefix="PS_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PS_Z1_" lnClass="PDIS" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PS_Z2_" lnClass="PDIS" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PS_Z3_" lnClass="PDIS" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PS_68_" lnClass="RPSB" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PS_67N_" lnClass="PTOC" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PS_78_" lnClass="PPAM" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PS_79_" lnClass="RREC" lnInst="1" doName="OpCls" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PS_SOTF_" lnClass="PSOF" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PS_TLP_" lnClass="PSCH" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PS_59_" lnClass="PTOV" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PS_ZP_" lnClass="PDIS" lnInst="1" doName="Op" daName="general" fc="ST" />
              <FCDA ldInst="LD0" prefix="PS_TLP_" lnClass="PSCH" lnInst="1" doName="RxTr1" daName="general" fc="ST" />
            </DataSet>
            <GSEControl type="GOOSE" appID="RELE_S_LT1LD0/LLN0$GO$SETGooseCB_1" confRev="20000" name="SETGooseCB_1" datSet="SETGooseDS_1">
              <IEDName>PATIO_4U4</IEDName>
              <IEDName>RDP_4U</IEDName>
              <IEDName>UAC_4U_P</IEDName>
              <IEDName>UAC_4U_S</IEDName>
            </GSEControl>
          </LN0>
          <LN prefix="PS_Z1_" lnClass="PDIS" inst="1" lnType="PDIS_CEMIG" />
          <LN prefix="PS_Z2_" lnClass="PDIS" inst="1" lnType="PDIS_CEMIG" />
          <LN prefix="PS_Z3_" lnClass="PDIS" inst="1" lnType="PDIS_CEMIG" />
          <LN prefix="PS_ZP_" lnClass="PDIS" inst="1" lnType="PDIS_CEMIG" />
          <LN prefix="PS_79_" lnClass="RREC" inst="1" lnType="RREC_CEMIG">
            <Inputs>
              <ExtRef desc="" iedName="PATIO_4U4" ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U4" ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Health" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U4" ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="CBOpCap" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="UAC_4U_P" ldInst="LD0" prefix="CP_86B_" lnClass="CILO" lnInst="1" doName="EnaCls" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="UAC_4U_P" ldInst="LD0" prefix="CP_86BF_" lnClass="CILO" lnInst="1" doName="EnaCls" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="UAC_4U_P" ldInst="LD0" prefix="CP_86TT_" lnClass="CILO" lnInst="1" doName="EnaCls" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="UAC_4U_S" ldInst="LD0" prefix="CS_86B_" lnClass="CILO" lnInst="1" doName="EnaCls" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="UAC_4U_S" ldInst="LD0" prefix="CS_86BF_" lnClass="CILO" lnInst="1" doName="EnaCls" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="UAC_4U_S" ldInst="LD0" prefix="CS_86TT_" lnClass="CILO" lnInst="1" doName="EnaCls" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
          <LN prefix="PS_68_" lnClass="RPSB" inst="1" lnType="RPSB_CEMIG" />
          <LN prefix="PS_TLP_" lnClass="PSCH" inst="1" lnType="PSCH_CEMIG" />
          <LN prefix="PS_94_" lnClass="PTRC" inst="1" lnType="PTRC_CEMIG" />
          <LN prefix="PS_59_" lnClass="PTOV" inst="1" lnType="PTOV_CEMIG" />
          <LN prefix="PS_SOTF_" lnClass="PSOF" inst="1" lnType="PSOF_CEMIG" />
          <LN prefix="PS_78_" lnClass="PPAM" inst="1" lnType="PPAM_CEMIG" />
          <LN prefix="PS_67N_" lnClass="PTOC" inst="1" lnType="PTOC_CEMIG" />
          <LN prefix="PS_51_" lnClass="PTOC" inst="1" lnType="PTOC_CEMIG" />
          <LN prefix="PS_50_" lnClass="PIOC" inst="1" lnType="PIOC_CEMIG" />
        </LDevice>
      </Server>
    </AccessPoint>
  </IED>
  <IED name="UAC_4U_P" desc="">
    <Private type="SchneiderElectric-SET-UserInterface">
      <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <ElectricalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="0" Height="0" />
              <Position X="0" Y="0" />
            </GraphicalProperties>
          </WorkArea>
        </ElectricalWorkAreas>
        <SystemWorkAreas>
          <WorkArea WorkAreaIdentification="05654ba5-3902-482b-9432-5fce4e2c1635">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="71" Y="185" />
            </GraphicalProperties>
          </WorkArea>
        </SystemWorkAreas>
        <LogicalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000003">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="True">
              <Size Width="129.806666666667" Height="40" />
              <Position X="-25.578125" Y="494.59375" />
            </GraphicalProperties>
          </WorkArea>
        </LogicalWorkAreas>
      </MetaInfosPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-ConsumerOrder">
      <ConsumerOrderPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <IEDNameOrder>
          <Item>
            <Key>
              <String>RELE_P_LT1</String>
            </Key>
            <Value>
              <Int32>0</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>RELE_S_LT1</String>
            </Key>
            <Value>
              <Int32>1</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>PATIO_4U4</String>
            </Key>
            <Value>
              <Int32>2</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>PATIO_4U3</String>
            </Key>
            <Value>
              <Int32>3</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>PATIO_4U5</String>
            </Key>
            <Value>
              <Int32>4</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>PATIO_4U8</String>
            </Key>
            <Value>
              <Int32>5</Int32>
            </Value>
          </Item>
        </IEDNameOrder>
      </ConsumerOrderPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-IEDProperties">
      <IEDPropertiesPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" ICDType="" IIDCIDType="" IEDClass="" ICDRevision="-1" SetRevision="0" IsInError="False" ElementType="IED" ElementFunction="None" />
    </Private>
    <Private type="SchneiderElectric-SET-IEDSubscribeEditorFilter">
      <IEDSubscribeEditorFilterPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" MappedLNChecked="False" PointToPointChecked="False" BufferMXChecked="False" BufferSTChecked="False" IsAutomaticQualityClient="False" />
    </Private>
    <Services>
      <ConfDataSet modify="true" maxAttributes="100" max="100" />
      <ConfReportControl bufMode="both" max="64" />
      <GOOSE max="64" />
      <ConfLNs fixPrefix="false" fixLnInst="false" />
      <ConfLdName />
    </Services>
    <AccessPoint name="AP1" clock="false">
      <Server>
        <Authentication />
        <LDevice inst="LD0">
          <LN0 lnClass="LLN0" inst="" lnType="SE_LLN0_SET_default_V001">
            <DataSet name="SETGooseDS_1" desc="">
              <FCDA ldInst="LD0" prefix="CP_86B_" lnClass="CILO" lnInst="1" doName="EnaCls" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="CP_86BF_" lnClass="CILO" lnInst="1" doName="EnaCls" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="CP_86TT_" lnClass="CILO" lnInst="1" doName="EnaCls" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="CP_CO_4U4_" lnClass="CSWI" lnInst="1" doName="Pos" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="CP_CO_4U3_" lnClass="CSWI" lnInst="1" doName="Pos" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="CP_CO_4U5_" lnClass="CSWI" lnInst="1" doName="Pos" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="CP_CO_4U8_" lnClass="CSWI" lnInst="1" doName="Pos" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="CP_CO_4U8T_" lnClass="CSWI" lnInst="1" doName="Pos" daName="stVal" fc="ST" />
            </DataSet>
            <GSEControl type="GOOSE" appID="UAC_4U_PLD0/LLN0$GO$SETGooseCB_1" confRev="10000" name="SETGooseCB_1" datSet="SETGooseDS_1">
              <IEDName>RELE_P_LT1</IEDName>
              <IEDName>RELE_S_LT1</IEDName>
              <IEDName>PATIO_4U4</IEDName>
              <IEDName>PATIO_4U3</IEDName>
              <IEDName>PATIO_4U5</IEDName>
              <IEDName>PATIO_4U8</IEDName>
            </GSEControl>
          </LN0>
          <LN prefix="CP_86B_" lnClass="CILO" inst="1" lnType="CILO_CEMIG" />
          <LN prefix="CP_86BF_" lnClass="CILO" inst="1" lnType="CILO_CEMIG" />
          <LN prefix="CP_86TT_" lnClass="CILO" inst="1" lnType="CILO_CEMIG">
            <Inputs>
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_TLP_" lnClass="PSCH" lnInst="1" doName="RxTr1" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_TLP_" lnClass="PSCH" lnInst="1" doName="RxTr1" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
          <LN prefix="CP_GRP_AL_" lnClass="CALH" inst="1" lnType="CALH_CEMIG" />
          <LN prefix="CP_MED_" lnClass="MMXU" inst="1" lnType="MMXU_CEMIG">
            <DataSet name="SETReportDS_1" desc="">
              <FCDA ldInst="LD0" prefix="CP_MED_" lnClass="MMXU" lnInst="1" doName="Beh" fc="ST" />
              <FCDA ldInst="LD0" prefix="CP_MED_" lnClass="MMXU" lnInst="1" doName="Health" fc="ST" />
            </DataSet>
            <ReportControl rptID="UAC_4U_PLD0/CP_MED_MMXU1$RP$SETReportCB_1" confRev="10000" buffered="false" indexed="true" name="SETReportCB_1" datSet="SETReportDS_1" desc="">
              <OptFields />
              <RptEnabled max="1" desc="">
                <ClientLN desc="" iedName="U_CENTRAL" ldInst="LD0" prefix="" lnClass="LLN0" lnInst="" />
              </RptEnabled>
            </ReportControl>
          </LN>
          <LN prefix="CP_AL_4U8T_" lnClass="CALH" inst="1" lnType="CALH_CEMIG" />
          <LN prefix="CP_CO_4U8T_" lnClass="CSWI" inst="1" lnType="CSWI_CEMIG" />
          <LN prefix="CP_IT_4U8T_" lnClass="CILO" inst="1" lnType="CILO_CEMIG">
            <Inputs>
              <ExtRef desc="" iedName="PATIO_4U8" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Health" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U8" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U8" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Loc" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
          <LN prefix="CP_AL_4U8_" lnClass="CALH" inst="1" lnType="CALH_CEMIG" />
          <LN prefix="CP_CO_4U5_" lnClass="CSWI" inst="1" lnType="CSWI_CEMIG" />
          <LN prefix="CP_IT_4U8_" lnClass="CILO" inst="1" lnType="CILO_CEMIG">
            <Inputs>
              <ExtRef desc="" iedName="PATIO_4U8" ldInst="LD0" prefix="PATIO_4U8_" lnClass="XSWI" lnInst="1" doName="Health" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U8" ldInst="LD0" prefix="PATIO_4U8_" lnClass="XSWI" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U8" ldInst="LD0" prefix="PATIO_4U8_" lnClass="XSWI" lnInst="1" doName="Loc" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
          <LN prefix="CP_AL_4U3_" lnClass="CALH" inst="1" lnType="CALH_CEMIG" />
          <LN prefix="CP_CO_4U4_" lnClass="CSWI" inst="1" lnType="CSWI_CEMIG" />
          <LN prefix="CP_IT_4U5_" lnClass="CILO" inst="1" lnType="CILO_CEMIG">
            <Inputs>
              <ExtRef desc="" iedName="PATIO_4U5" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U5" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Health" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U5" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Loc" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
          <LN prefix="CP_AL_4U5_" lnClass="CALH" inst="1" lnType="CALH_CEMIG" />
          <LN prefix="CP_CO_4U3_" lnClass="CSWI" inst="1" lnType="CSWI_CEMIG" />
          <LN prefix="CP_IT_4U3_" lnClass="CILO" inst="1" lnType="CILO_CEMIG">
            <Inputs>
              <ExtRef desc="" iedName="PATIO_4U3" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Loc" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U3" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U3" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Health" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
          <LN prefix="CP_AL_4U4_" lnClass="CALH" inst="1" lnType="CALH_CEMIG" />
          <LN prefix="CP_CO_4U8_" lnClass="CSWI" inst="1" lnType="CSWI_CEMIG" />
          <LN prefix="CP_IT_4U4_" lnClass="CILO" inst="1" lnType="CILO_CEMIG">
            <Inputs>
              <ExtRef desc="" iedName="PATIO_4U4" ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U4" ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Dsc" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U4" ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="CBOpCap" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U4" ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Loc" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U4" ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Health" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
          <LN prefix="CP_25_" lnClass="RSYN" inst="1" lnType="RSYN_CEMIG" />
        </LDevice>
      </Server>
    </AccessPoint>
  </IED>
  <IED name="ICE_2" desc="">
    <Private type="SchneiderElectric-SET-UserInterface">
      <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <ElectricalWorkAreas />
        <SystemWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000002">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="64" Height="35" />
              <Position X="407.15625" Y="406.7313125" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="5570cec2-94b6-41fa-bf42-9026ab8f5b26">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="50" Height="50" />
              <Position X="125" Y="50" />
            </GraphicalProperties>
          </WorkArea>
        </SystemWorkAreas>
        <LogicalWorkAreas />
      </MetaInfosPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-IEDProperties">
      <IEDPropertiesPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" ICDType="" IIDCIDType="" IEDClass="" ICDRevision="-1" SetRevision="0" IsInError="False" ElementType="ICE" ElementFunction="None" />
    </Private>
    <Private type="SchneiderElectric-SET-InterConnectedEquipment">
      <ICEPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" GooseFlow="False" ReportFlow="True" />
    </Private>
    <AccessPoint name="AP1_ICE_2" />
    <AccessPoint name="AP2_ICE_2" />
  </IED>
  <IED name="RDP_4U" desc="">
    <Private type="SchneiderElectric-SET-UserInterface">
      <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <ElectricalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="0" Height="0" />
              <Position X="0" Y="0" />
            </GraphicalProperties>
          </WorkArea>
        </ElectricalWorkAreas>
        <SystemWorkAreas>
          <WorkArea WorkAreaIdentification="05654ba5-3902-482b-9432-5fce4e2c1635">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="445" Y="201" />
            </GraphicalProperties>
          </WorkArea>
        </SystemWorkAreas>
        <LogicalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000003">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="True">
              <Size Width="117.003333333333" Height="40" />
              <Position X="1189.265625" Y="321.9375" />
            </GraphicalProperties>
          </WorkArea>
        </LogicalWorkAreas>
      </MetaInfosPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-IEDProperties">
      <IEDPropertiesPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" ICDType="" IIDCIDType="" IEDClass="" ICDRevision="-1" SetRevision="0" IsInError="False" ElementType="IED" ElementFunction="None" />
    </Private>
    <Private type="SchneiderElectric-SET-IEDSubscribeEditorFilter">
      <IEDSubscribeEditorFilterPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" MappedLNChecked="False" PointToPointChecked="False" BufferMXChecked="False" BufferSTChecked="False" IsAutomaticQualityClient="False" />
    </Private>
    <Services>
      <ConfDataSet modify="true" maxAttributes="100" max="100" />
      <ConfReportControl bufMode="both" max="64" />
      <GOOSE max="64" />
      <ConfLNs fixPrefix="false" fixLnInst="false" />
      <ConfLdName />
    </Services>
    <AccessPoint name="AP1" clock="false">
      <Server>
        <Authentication />
        <LDevice inst="LD0">
          <LN0 lnClass="LLN0" inst="" lnType="SE_LLN0_SET_default_V001" />
          <LN prefix="RDP_" lnClass="RADR" inst="1" lnType="RADR_CEMIG" />
          <LN prefix="RDP_" lnClass="RBDR" inst="1" lnType="RBDR_CEMIG">
            <Inputs>
              <ExtRef desc="" iedName="PATIO_4U4" ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="phsA" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="phsB" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="phsC" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_Z1_" lnClass="PDIS" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_Z2_" lnClass="PDIS" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_Z3_" lnClass="PDIS" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_ZP_" lnClass="PDIS" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_67N_" lnClass="PTOC" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_59_" lnClass="PTOV" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_79_" lnClass="RREC" lnInst="1" doName="OpCls" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_78_" lnClass="PPAM" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_68_" lnClass="RPSB" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_SOTF_" lnClass="PSOF" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_TLP_" lnClass="PSCH" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="phsA" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="phsB" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="phsC" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_Z1_" lnClass="PDIS" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_Z2_" lnClass="PDIS" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_Z3_" lnClass="PDIS" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_68_" lnClass="RPSB" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_67N_" lnClass="PTOC" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_78_" lnClass="PPAM" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_79_" lnClass="RREC" lnInst="1" doName="OpCls" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_SOTF_" lnClass="PSOF" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_TLP_" lnClass="PSCH" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_59_" lnClass="PTOV" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_ZP_" lnClass="PDIS" lnInst="1" doName="Op" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
          <LN prefix="RDP_" lnClass="RDRE" inst="1" lnType="RDRE_CEMIG" />
        </LDevice>
      </Server>
    </AccessPoint>
  </IED>
  <IED name="U_CENTRAL" desc="">
    <Private type="SchneiderElectric-SET-IEDSubscribeEditorFilter">
      <IEDSubscribeEditorFilterPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" MappedLNChecked="False" PointToPointChecked="False" BufferMXChecked="False" BufferSTChecked="False" IsAutomaticQualityClient="False" />
    </Private>
    <Private type="SchneiderElectric-SET-IEDProperties">
      <IEDPropertiesPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" ICDType="" IIDCIDType="" IEDClass="" ICDRevision="-1" SetRevision="0" IsInError="False" ElementType="IED" ElementFunction="None" />
    </Private>
    <Private type="SchneiderElectric-SET-UserInterface">
      <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <ElectricalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="0" Height="0" />
              <Position X="0" Y="0" />
            </GraphicalProperties>
          </WorkArea>
        </ElectricalWorkAreas>
        <SystemWorkAreas>
          <WorkArea WorkAreaIdentification="da9b4ce3-826f-4bd4-a5cc-55623ea380fc">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="50" Height="50" />
              <Position X="650" Y="50" />
            </GraphicalProperties>
          </WorkArea>
        </SystemWorkAreas>
        <LogicalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000003">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="True">
              <Size Width="137.553333333333" Height="40" />
              <Position X="-299.39453125" Y="1082.80078125" />
            </GraphicalProperties>
          </WorkArea>
        </LogicalWorkAreas>
      </MetaInfosPrivates>
    </Private>
    <Services>
      <ConfDataSet modify="true" maxAttributes="100" max="100" />
      <ConfReportControl bufMode="both" max="64" />
      <GOOSE max="64" />
      <ConfLNs fixPrefix="false" fixLnInst="false" />
      <ConfLdName />
    </Services>
    <AccessPoint name="AP1" clock="false">
      <Server>
        <Authentication />
        <LDevice inst="LD0">
          <LN0 lnClass="LLN0" inst="" lnType="SE_LLN0_SET_default_V001" />
          <LN prefix="" lnClass="IHMI" inst="1" lnType="SE_IHMI_SET_default_V001" />
          <LN prefix="" lnClass="ITCI" inst="1" lnType="SE_ITCI_SET_default_V001" />
        </LDevice>
      </Server>
    </AccessPoint>
  </IED>
  <IED name="PATIO_4U3" desc="">
    <Private type="SchneiderElectric-SET-IEDSubscribeEditorFilter">
      <IEDSubscribeEditorFilterPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" MappedLNChecked="False" PointToPointChecked="False" BufferMXChecked="False" BufferSTChecked="False" IsAutomaticQualityClient="False" />
    </Private>
    <Private type="SchneiderElectric-SET-UserInterface">
      <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <ElectricalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="0" Height="0" />
              <Position X="0" Y="0" />
            </GraphicalProperties>
          </WorkArea>
        </ElectricalWorkAreas>
        <SystemWorkAreas>
          <WorkArea WorkAreaIdentification="da9b4ce3-826f-4bd4-a5cc-55623ea380fc">
            <GraphicalProperties DuplicateCount="1" IsPlaced="False" IsExpanded="False">
              <Size Width="50" Height="50" />
              <Position X="125" Y="50" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="05654ba5-3902-482b-9432-5fce4e2c1635">
            <GraphicalProperties DuplicateCount="1" IsPlaced="False" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="251" Y="392" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="31e25233-963c-4dc6-b5f2-82cf3fbc3e9f">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="124" Y="207" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="5570cec2-94b6-41fa-bf42-9026ab8f5b26">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="209.95" Y="341.37890625" />
            </GraphicalProperties>
          </WorkArea>
        </SystemWorkAreas>
        <LogicalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000003">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="True">
              <Size Width="134.15" Height="40" />
              <Position X="-82.578125" Y="868.65625" />
            </GraphicalProperties>
          </WorkArea>
        </LogicalWorkAreas>
      </MetaInfosPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-IEDProperties">
      <IEDPropertiesPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" ICDType="" IIDCIDType="" IEDClass="" ICDRevision="-1" SetRevision="0" IsInError="False" ElementType="IED" ElementFunction="None" />
    </Private>
    <Private type="SchneiderElectric-SET-ConsumerOrder">
      <ConsumerOrderPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <IEDNameOrder>
          <Item>
            <Key>
              <String>UAC_4U</String>
            </Key>
            <Value>
              <Int32>0</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>UAC_4U_P</String>
            </Key>
            <Value>
              <Int32>0</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>UAC_4U_S</String>
            </Key>
            <Value>
              <Int32>1</Int32>
            </Value>
          </Item>
        </IEDNameOrder>
      </ConsumerOrderPrivates>
    </Private>
    <Services>
      <ConfDataSet modify="true" maxAttributes="100" max="100" />
      <ConfReportControl bufMode="both" max="64" />
      <GOOSE max="64" />
      <ConfLNs fixPrefix="false" fixLnInst="false" />
      <ConfLdName />
    </Services>
    <AccessPoint name="AP1" clock="false">
      <Server>
        <Authentication />
        <LDevice inst="LD0">
          <LN0 lnClass="LLN0" inst="" lnType="SE_LLN0_SET_default_V001">
            <DataSet name="SETGooseDS_1" desc="">
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Pos" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Loc" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Health" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Health" daName="stVal" fc="ST" />
            </DataSet>
            <GSEControl type="GOOSE" appID="PATIO_4U3LD0/LLN0$GO$SETGooseCB_1" confRev="20000" name="SETGooseCB_1" datSet="SETGooseDS_1">
              <IEDName>UAC_4U_P</IEDName>
              <IEDName>UAC_4U_S</IEDName>
            </GSEControl>
          </LN0>
          <LN prefix="PATIO_" lnClass="XSWI" inst="1" lnType="XSWI_CEMIG">
            <DataSet name="SETReportDS_1" desc="">
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="SwTyp" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Loc" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="OpCnt" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Pos" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Beh" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Health" fc="ST" />
            </DataSet>
            <ReportControl rptID="PATIO_4U3LD0/PATIO_XSWI1$RP$SETReportCB_1" confRev="10000" buffered="false" indexed="true" name="SETReportCB_1" datSet="SETReportDS_1" desc="">
              <OptFields />
              <RptEnabled max="1" desc="">
                <ClientLN desc="" iedName="U_CENTRAL" ldInst="LD0" prefix="" lnClass="LLN0" lnInst="" />
              </RptEnabled>
            </ReportControl>
            <Inputs>
              <ExtRef desc="" iedName="UAC_4U_P" ldInst="LD0" prefix="CP_CO_4U3_" lnClass="CSWI" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="UAC_4U_S" ldInst="LD0" prefix="CS_CO_4U3_" lnClass="CSWI" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
        </LDevice>
      </Server>
    </AccessPoint>
  </IED>
  <IED name="RELE_BF_4U_S" desc="">
    <Private type="SchneiderElectric-SET-UserInterface">
      <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <ElectricalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="0" Height="0" />
              <Position X="0" Y="0" />
            </GraphicalProperties>
          </WorkArea>
        </ElectricalWorkAreas>
        <SystemWorkAreas>
          <WorkArea WorkAreaIdentification="aaeebf5a-e239-4b91-8239-2a04f18f019a">
            <GraphicalProperties DuplicateCount="1" IsPlaced="False" IsExpanded="False">
              <Size Width="0" Height="0" />
              <Position X="0" Y="0" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="05654ba5-3902-482b-9432-5fce4e2c1635">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="285" Y="220" />
            </GraphicalProperties>
          </WorkArea>
        </SystemWorkAreas>
        <LogicalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000003">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="True">
              <Size Width="148.733333333333" Height="40" />
              <Position X="1089.640625" Y="780.203125" />
            </GraphicalProperties>
          </WorkArea>
        </LogicalWorkAreas>
      </MetaInfosPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-IEDProperties">
      <IEDPropertiesPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" ICDType="" IIDCIDType="" IEDClass="" ICDRevision="-1" SetRevision="0" IsInError="False" ElementType="IED" ElementFunction="None" />
    </Private>
    <Private type="SchneiderElectric-SET-IEDSubscribeEditorFilter">
      <IEDSubscribeEditorFilterPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" MappedLNChecked="False" PointToPointChecked="False" BufferMXChecked="False" BufferSTChecked="False" IsAutomaticQualityClient="False" />
    </Private>
    <Services>
      <ConfDataSet modify="true" maxAttributes="100" max="100" />
      <ConfReportControl bufMode="both" max="64" />
      <GOOSE max="64" />
      <ConfLNs fixPrefix="false" fixLnInst="false" />
      <ConfLdName />
    </Services>
    <AccessPoint name="AP1" clock="false">
      <Server>
        <Authentication />
        <LDevice inst="LD0">
          <LN0 lnClass="LLN0" inst="" lnType="SE_LLN0_SET_default_V001" />
          <LN prefix="PS_BF_" lnClass="RBRF" inst="1" lnType="RBRF_CEMIG">
            <Inputs>
              <ExtRef desc="" iedName="PATIO_4U4" ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
          <LN prefix="PS_BF94_" lnClass="PTRC" inst="1" lnType="PTRC_CEMIG" />
        </LDevice>
      </Server>
    </AccessPoint>
  </IED>
  <IED name="UAC_4U_S" desc="">
    <Private type="SchneiderElectric-SET-UserInterface">
      <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <ElectricalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="0" Height="0" />
              <Position X="0" Y="0" />
            </GraphicalProperties>
          </WorkArea>
        </ElectricalWorkAreas>
        <SystemWorkAreas>
          <WorkArea WorkAreaIdentification="05654ba5-3902-482b-9432-5fce4e2c1635">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="70" Y="224" />
            </GraphicalProperties>
          </WorkArea>
        </SystemWorkAreas>
        <LogicalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000003">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="True">
              <Size Width="129.46" Height="40" />
              <Position X="150.84375" Y="500.421875" />
            </GraphicalProperties>
          </WorkArea>
        </LogicalWorkAreas>
      </MetaInfosPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-ConsumerOrder">
      <ConsumerOrderPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <IEDNameOrder>
          <Item>
            <Key>
              <String>RELE_P_LT1</String>
            </Key>
            <Value>
              <Int32>0</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>RELE_S_LT1</String>
            </Key>
            <Value>
              <Int32>1</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>PATIO_4U4</String>
            </Key>
            <Value>
              <Int32>2</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>PATIO_4U3</String>
            </Key>
            <Value>
              <Int32>3</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>PATIO_4U5</String>
            </Key>
            <Value>
              <Int32>4</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>PATIO_4U8</String>
            </Key>
            <Value>
              <Int32>5</Int32>
            </Value>
          </Item>
        </IEDNameOrder>
      </ConsumerOrderPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-IEDProperties">
      <IEDPropertiesPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" ICDType="" IIDCIDType="" IEDClass="" ICDRevision="-1" SetRevision="0" IsInError="False" ElementType="IED" ElementFunction="None" />
    </Private>
    <Private type="SchneiderElectric-SET-IEDSubscribeEditorFilter">
      <IEDSubscribeEditorFilterPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" MappedLNChecked="False" PointToPointChecked="False" BufferMXChecked="False" BufferSTChecked="False" IsAutomaticQualityClient="False" />
    </Private>
    <Services>
      <ConfDataSet modify="true" maxAttributes="100" max="100" />
      <ConfReportControl bufMode="both" max="64" />
      <GOOSE max="64" />
      <ConfLNs fixPrefix="false" fixLnInst="false" />
      <ConfLdName />
    </Services>
    <AccessPoint name="AP1" clock="false">
      <Server>
        <Authentication />
        <LDevice inst="LD0">
          <LN0 lnClass="LLN0" inst="" lnType="SE_LLN0_SET_default_V001">
            <DataSet name="SETGooseDS_1" desc="">
              <FCDA ldInst="LD0" prefix="CS_86B_" lnClass="CILO" lnInst="1" doName="EnaCls" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="CS_86BF_" lnClass="CILO" lnInst="1" doName="EnaCls" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="CS_86TT_" lnClass="CILO" lnInst="1" doName="EnaCls" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="CS_CO_4U4_" lnClass="CSWI" lnInst="1" doName="Pos" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="CS_CO_4U3_" lnClass="CSWI" lnInst="1" doName="Pos" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="CS_CO_4U5_" lnClass="CSWI" lnInst="1" doName="Pos" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="CS_CO_4U8_" lnClass="CSWI" lnInst="1" doName="Pos" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="CS_CO_4U8T_" lnClass="CSWI" lnInst="1" doName="Pos" daName="stVal" fc="ST" />
            </DataSet>
            <GSEControl type="GOOSE" appID="UAC_4U_SLD0/LLN0$GO$SETGooseCB_1" confRev="10000" name="SETGooseCB_1" datSet="SETGooseDS_1">
              <IEDName>RELE_P_LT1</IEDName>
              <IEDName>RELE_S_LT1</IEDName>
              <IEDName>PATIO_4U4</IEDName>
              <IEDName>PATIO_4U3</IEDName>
              <IEDName>PATIO_4U5</IEDName>
              <IEDName>PATIO_4U8</IEDName>
            </GSEControl>
          </LN0>
          <LN prefix="CS_86B_" lnClass="CILO" inst="1" lnType="CILO_CEMIG" />
          <LN prefix="CS_86BF_" lnClass="CILO" inst="1" lnType="CILO_CEMIG" />
          <LN prefix="CS_86TT_" lnClass="CILO" inst="1" lnType="CILO_CEMIG">
            <Inputs>
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_TLP_" lnClass="PSCH" lnInst="1" doName="RxTr1" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_TLP_" lnClass="PSCH" lnInst="1" doName="RxTr1" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
          <LN prefix="CS_GRP_AL_" lnClass="CALH" inst="1" lnType="CALH_CEMIG" />
          <LN prefix="CS_MED_" lnClass="MMXU" inst="1" lnType="MMXU_CEMIG">
            <DataSet name="SETReportDS_1" desc="">
              <FCDA ldInst="LD0" prefix="CS_MED_" lnClass="MMXU" lnInst="1" doName="Beh" fc="ST" />
              <FCDA ldInst="LD0" prefix="CS_MED_" lnClass="MMXU" lnInst="1" doName="Health" fc="ST" />
            </DataSet>
            <ReportControl rptID="UAC_4U_SLD0/CS_MED_MMXU1$RP$SETReportCB_1" confRev="10000" buffered="false" indexed="true" name="SETReportCB_1" datSet="SETReportDS_1" desc="">
              <OptFields />
              <RptEnabled max="1" desc="">
                <ClientLN desc="" iedName="U_CENTRAL" ldInst="LD0" prefix="" lnClass="LLN0" lnInst="" />
              </RptEnabled>
            </ReportControl>
          </LN>
          <LN prefix="CS_AL_4U8T_" lnClass="CALH" inst="1" lnType="CALH_CEMIG" />
          <LN prefix="CS_CO_4U8T_" lnClass="CSWI" inst="1" lnType="CSWI_CEMIG" />
          <LN prefix="CS_IT_4U8T_" lnClass="CILO" inst="1" lnType="CILO_CEMIG">
            <Inputs>
              <ExtRef desc="" iedName="PATIO_4U8" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Health" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U8" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U8" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Loc" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
          <LN prefix="CS_AL_4U4_" lnClass="CALH" inst="1" lnType="CALH_CEMIG" />
          <LN prefix="CS_CO_4U3_" lnClass="CSWI" inst="1" lnType="CSWI_CEMIG" />
          <LN prefix="CS_IT_4U5_" lnClass="CILO" inst="1" lnType="CILO_CEMIG">
            <Inputs>
              <ExtRef desc="" iedName="PATIO_4U5" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U5" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Loc" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U5" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Health" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
          <LN prefix="CS_AL_4U3_" lnClass="CALH" inst="1" lnType="CALH_CEMIG" />
          <LN prefix="CS_CO_4U8_" lnClass="CSWI" inst="1" lnType="CSWI_CEMIG" />
          <LN prefix="CS_IT_4U8_" lnClass="CILO" inst="1" lnType="CILO_CEMIG">
            <Inputs>
              <ExtRef desc="" iedName="PATIO_4U8" ldInst="LD0" prefix="PATIO_4U8_" lnClass="XSWI" lnInst="1" doName="Health" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U8" ldInst="LD0" prefix="PATIO_4U8_" lnClass="XSWI" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U8" ldInst="LD0" prefix="PATIO_4U8_" lnClass="XSWI" lnInst="1" doName="Loc" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
          <LN prefix="CS_AL_4U8_" lnClass="CALH" inst="1" lnType="CALH_CEMIG" />
          <LN prefix="CS_CO_4U5_" lnClass="CSWI" inst="1" lnType="CSWI_CEMIG" />
          <LN prefix="CS_IT_4U4_" lnClass="CILO" inst="1" lnType="CILO_CEMIG">
            <Inputs>
              <ExtRef desc="" iedName="PATIO_4U4" ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U4" ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Dsc" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U4" ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="CBOpCap" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U4" ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Loc" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U4" ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Health" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
          <LN prefix="CS_AL_4U5_" lnClass="CALH" inst="1" lnType="CALH_CEMIG" />
          <LN prefix="CS_CO_4U4_" lnClass="CSWI" inst="1" lnType="CSWI_CEMIG" />
          <LN prefix="CS_IT_4U3_" lnClass="CILO" inst="1" lnType="CILO_CEMIG">
            <Inputs>
              <ExtRef desc="" iedName="PATIO_4U3" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Loc" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U3" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="PATIO_4U3" ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Health" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
          <LN prefix="CS_25_" lnClass="RSYN" inst="1" lnType="RSYN_CEMIG" />
        </LDevice>
      </Server>
    </AccessPoint>
  </IED>
  <IED name="PATIO_4U4" desc="">
    <Private type="SchneiderElectric-SET-ConsumerOrder">
      <ConsumerOrderPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <IEDNameOrder>
          <Item>
            <Key>
              <String>UAC_4U_P</String>
            </Key>
            <Value>
              <Int32>0</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>UAC_4U_S</String>
            </Key>
            <Value>
              <Int32>1</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>RELE_BF_4U_S</String>
            </Key>
            <Value>
              <Int32>2</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>RELE_BF_4U_P</String>
            </Key>
            <Value>
              <Int32>6</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>RELE_P_LT1</String>
            </Key>
            <Value>
              <Int32>3</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>RELE_S_LT1</String>
            </Key>
            <Value>
              <Int32>4</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>RDP_4U</String>
            </Key>
            <Value>
              <Int32>5</Int32>
            </Value>
          </Item>
        </IEDNameOrder>
      </ConsumerOrderPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-IEDSubscribeEditorFilter">
      <IEDSubscribeEditorFilterPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" MappedLNChecked="False" PointToPointChecked="False" BufferMXChecked="False" BufferSTChecked="False" IsAutomaticQualityClient="False" />
    </Private>
    <Private type="SchneiderElectric-SET-IEDProperties">
      <IEDPropertiesPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" ICDType="" IIDCIDType="" IEDClass="" ICDRevision="-1" SetRevision="0" IsInError="False" ElementType="IED" ElementFunction="None" />
    </Private>
    <Private type="SchneiderElectric-SET-UserInterface">
      <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <ElectricalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="0" Height="0" />
              <Position X="0" Y="0" />
            </GraphicalProperties>
          </WorkArea>
        </ElectricalWorkAreas>
        <SystemWorkAreas>
          <WorkArea WorkAreaIdentification="da9b4ce3-826f-4bd4-a5cc-55623ea380fc">
            <GraphicalProperties DuplicateCount="1" IsPlaced="False" IsExpanded="False">
              <Size Width="50" Height="50" />
              <Position X="275" Y="50" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="05654ba5-3902-482b-9432-5fce4e2c1635">
            <GraphicalProperties DuplicateCount="1" IsPlaced="False" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="274" Y="341" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="31e25233-963c-4dc6-b5f2-82cf3fbc3e9f">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="128" Y="257" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="5570cec2-94b6-41fa-bf42-9026ab8f5b26">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="308.7" Y="342.62890625" />
            </GraphicalProperties>
          </WorkArea>
        </SystemWorkAreas>
        <LogicalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000003">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="True">
              <Size Width="134.15" Height="40" />
              <Position X="647.5" Y="877.9375" />
            </GraphicalProperties>
          </WorkArea>
        </LogicalWorkAreas>
      </MetaInfosPrivates>
    </Private>
    <Services>
      <ConfDataSet modify="true" maxAttributes="100" max="100" />
      <ConfReportControl bufMode="both" max="64" />
      <GOOSE max="64" />
      <ConfLNs fixPrefix="false" fixLnInst="false" />
      <ConfLdName />
    </Services>
    <AccessPoint name="AP1" clock="false">
      <Server>
        <Authentication />
        <LDevice inst="LD0">
          <LN0 lnClass="LLN0" inst="" lnType="SE_LLN0_SET_default_V001">
            <DataSet name="SETGooseDS_1" desc="">
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Dsc" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Pos" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="CBOpCap" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Loc" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Health" daName="stVal" fc="ST" />
            </DataSet>
            <GSEControl type="GOOSE" appID="PATIO_4U4LD0/LLN0$GO$SETGooseCB_1" confRev="20000" name="SETGooseCB_1" datSet="SETGooseDS_1">
              <IEDName>UAC_4U_P</IEDName>
              <IEDName>UAC_4U_S</IEDName>
              <IEDName>RELE_BF_4U_S</IEDName>
              <IEDName>RELE_BF_4U_P</IEDName>
              <IEDName>RELE_P_LT1</IEDName>
              <IEDName>RELE_S_LT1</IEDName>
              <IEDName>RDP_4U</IEDName>
            </GSEControl>
          </LN0>
          <LN prefix="PATIO_" lnClass="XCBR" inst="1" lnType="XCBR_CEMIG">
            <DataSet name="SETReportDS_1" desc="">
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Loc" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="OpCnt" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Pos" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Beh" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="CBOpCap" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Dsc" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XCBR" lnInst="1" doName="Health" fc="ST" />
            </DataSet>
            <ReportControl rptID="PATIO_4U4LD0/PATIO_XCBR1$RP$SETReportCB_1" confRev="10000" buffered="false" indexed="true" name="SETReportCB_1" datSet="SETReportDS_1" desc="">
              <OptFields />
              <RptEnabled max="1" desc="">
                <ClientLN desc="" iedName="U_CENTRAL" ldInst="LD0" prefix="" lnClass="LLN0" lnInst="" />
              </RptEnabled>
            </ReportControl>
            <Inputs>
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="phsA" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="phsB" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="phsC" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="phsA" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="phsB" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_S_LT1" ldInst="LD0" prefix="PS_94_" lnClass="PTRC" lnInst="1" doName="Op" daName="phsC" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="UAC_4U_S" ldInst="LD0" prefix="CS_CO_4U4_" lnClass="CSWI" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="UAC_4U_P" ldInst="LD0" prefix="CP_CO_4U4_" lnClass="CSWI" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="RELE_P_LT1" ldInst="LD0" prefix="PP_79_5U4" lnClass="RREC" lnInst="1" doName="OpCls" daName="general" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
        </LDevice>
      </Server>
    </AccessPoint>
  </IED>
  <IED name="ICE_3" desc="">
    <Private type="SchneiderElectric-SET-UserInterface">
      <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <ElectricalWorkAreas />
        <SystemWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000002">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="64" Height="35" />
              <Position X="194.2625" Y="512" />
            </GraphicalProperties>
          </WorkArea>
        </SystemWorkAreas>
        <LogicalWorkAreas />
      </MetaInfosPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-IEDProperties">
      <IEDPropertiesPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" ICDType="" IIDCIDType="" IEDClass="" ICDRevision="-1" SetRevision="0" IsInError="False" ElementType="ICE" ElementFunction="None" />
    </Private>
    <Private type="SchneiderElectric-SET-InterConnectedEquipment">
      <ICEPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" GooseFlow="True" ReportFlow="True" />
    </Private>
    <Private type="SchneiderElectric-SET-IEDSubscribeEditorFilter">
      <IEDSubscribeEditorFilterPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" MappedLNChecked="False" PointToPointChecked="False" BufferMXChecked="False" BufferSTChecked="False" IsAutomaticQualityClient="False" />
    </Private>
    <AccessPoint name="AP1_ICE_3" clock="false" />
    <AccessPoint name="AP2_ICE_3" />
  </IED>
  <IED name="RELE_BF_5U_P" desc="">
    <Private type="SchneiderElectric-SET-ControlManagementData">
      <ControlManagementPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" SBOTimeOUT="-1" SBOClass="Undefined" CTLModel="Undefined" />
    </Private>
    <Private type="SchneiderElectric-SET-IEDSubscribeEditorFilter">
      <IEDSubscribeEditorFilterPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" MappedLNChecked="False" PointToPointChecked="False" BufferMXChecked="False" BufferSTChecked="False" IsAutomaticQualityClient="False" />
    </Private>
    <Private type="SchneiderElectric-SET-IEDProperties">
      <IEDPropertiesPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" ICDType="" IIDCIDType="" IEDClass="" ICDRevision="-1" SetRevision="0" IsInError="False" ElementType="IED" ElementFunction="None" />
    </Private>
    <Private type="SchneiderElectric-SET-UserInterface">
      <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <ElectricalWorkAreas />
        <SystemWorkAreas>
          <WorkArea WorkAreaIdentification="3c25b338-3eed-4c5a-98f7-99547bf9429f">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="353.7" Y="542.62890625" />
            </GraphicalProperties>
          </WorkArea>
        </SystemWorkAreas>
        <LogicalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000003">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="True">
              <Size Width="149.08" Height="40" />
              <Position X="1125.07079101562" Y="1046.87232421875" />
            </GraphicalProperties>
          </WorkArea>
        </LogicalWorkAreas>
      </MetaInfosPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-ConsumerOrder">
      <ConsumerOrderPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <IEDNameOrder>
          <Item>
            <Key>
              <String>RELE_BF_4U_P</String>
            </Key>
            <Value>
              <Int32>0</Int32>
            </Value>
          </Item>
        </IEDNameOrder>
      </ConsumerOrderPrivates>
    </Private>
    <Services>
      <ConfDataSet modify="true" maxAttributes="100" max="100" />
      <ConfReportControl bufMode="both" max="64" />
      <GOOSE max="64" />
      <ConfLNs fixPrefix="false" fixLnInst="false" />
      <ConfLdName />
    </Services>
    <AccessPoint name="AP1" clock="false">
      <Server>
        <Authentication />
        <LDevice inst="LD0">
          <LN0 lnClass="LLN0" inst="" lnType="SE_LLN0_SET_default_V001">
            <DataSet name="SETGooseDS_1" desc="">
              <FCDA ldInst="LD0" prefix="PP_BF94_" lnClass="PTRC" lnInst="1" doName="Op" daName="general" fc="ST" />
            </DataSet>
            <GSEControl type="GOOSE" appID="RELE_BF_5U_PLD0/LLN0$GO$SETGooseCB_1" confRev="10000" name="SETGooseCB_1" datSet="SETGooseDS_1">
              <IEDName>RELE_BF_4U_P</IEDName>
            </GSEControl>
          </LN0>
          <LN prefix="PP_BF_" lnClass="RBRF" inst="1" lnType="RBRF_CEMIG" />
          <LN prefix="PP_BF94_" lnClass="PTRC" inst="1" lnType="PTRC_CEMIG" />
        </LDevice>
      </Server>
    </AccessPoint>
  </IED>
  <IED name="PATIO_4U8" desc="">
    <Private type="SchneiderElectric-SET-IEDSubscribeEditorFilter">
      <IEDSubscribeEditorFilterPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" MappedLNChecked="False" PointToPointChecked="False" BufferMXChecked="False" BufferSTChecked="False" IsAutomaticQualityClient="False" />
    </Private>
    <Private type="SchneiderElectric-SET-UserInterface">
      <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <ElectricalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="0" Height="0" />
              <Position X="0" Y="0" />
            </GraphicalProperties>
          </WorkArea>
        </ElectricalWorkAreas>
        <SystemWorkAreas>
          <WorkArea WorkAreaIdentification="da9b4ce3-826f-4bd4-a5cc-55623ea380fc">
            <GraphicalProperties DuplicateCount="1" IsPlaced="False" IsExpanded="False">
              <Size Width="50" Height="50" />
              <Position X="49" Y="125" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="05654ba5-3902-482b-9432-5fce4e2c1635">
            <GraphicalProperties DuplicateCount="1" IsPlaced="False" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="168" Y="477" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="31e25233-963c-4dc6-b5f2-82cf3fbc3e9f">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="128" Y="352" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="5570cec2-94b6-41fa-bf42-9026ab8f5b26">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="509.95" Y="343.87890625" />
            </GraphicalProperties>
          </WorkArea>
        </SystemWorkAreas>
        <LogicalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000003">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="True">
              <Size Width="134.15" Height="40" />
              <Position X="321.34375" Y="866.953125" />
            </GraphicalProperties>
          </WorkArea>
        </LogicalWorkAreas>
      </MetaInfosPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-IEDProperties">
      <IEDPropertiesPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" ICDType="" IIDCIDType="" IEDClass="" ICDRevision="-1" SetRevision="0" IsInError="False" ElementType="IED" ElementFunction="None" />
    </Private>
    <Private type="SchneiderElectric-SET-ConsumerOrder">
      <ConsumerOrderPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <IEDNameOrder>
          <Item>
            <Key>
              <String>UAC_4U_P</String>
            </Key>
            <Value>
              <Int32>0</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>UAC_4U_S</String>
            </Key>
            <Value>
              <Int32>1</Int32>
            </Value>
          </Item>
        </IEDNameOrder>
      </ConsumerOrderPrivates>
    </Private>
    <Services>
      <ConfDataSet modify="true" maxAttributes="100" max="100" />
      <ConfReportControl bufMode="both" max="64" />
      <GOOSE max="64" />
      <ConfLNs fixPrefix="false" fixLnInst="false" />
      <ConfLdName />
    </Services>
    <AccessPoint name="AP1" clock="false">
      <Server>
        <Authentication />
        <LDevice inst="LD0">
          <LN0 lnClass="LLN0" inst="" lnType="SE_LLN0_SET_default_V001">
            <DataSet name="SETGooseDS_1" desc="">
              <FCDA ldInst="LD0" prefix="PATIO_4U8_" lnClass="XSWI" lnInst="1" doName="Health" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_4U8_" lnClass="XSWI" lnInst="1" doName="Pos" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_4U8_" lnClass="XSWI" lnInst="1" doName="Loc" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Health" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Pos" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Loc" daName="stVal" fc="ST" />
            </DataSet>
            <GSEControl type="GOOSE" appID="PATIO_4U8LD0/LLN0$GO$SETGooseCB_1" confRev="10000" name="SETGooseCB_1" datSet="SETGooseDS_1">
              <IEDName>UAC_4U_P</IEDName>
              <IEDName>UAC_4U_S</IEDName>
            </GSEControl>
          </LN0>
          <LN prefix="PATIO_4U8_" lnClass="XSWI" inst="1" lnType="XSWI_CEMIG">
            <DataSet name="SETReportDS_1" desc="">
              <FCDA ldInst="LD0" prefix="PATIO_4U8_" lnClass="XSWI" lnInst="1" doName="SwTyp" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_4U8_" lnClass="XSWI" lnInst="1" doName="Loc" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_4U8_" lnClass="XSWI" lnInst="1" doName="OpCnt" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_4U8_" lnClass="XSWI" lnInst="1" doName="Pos" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_4U8_" lnClass="XSWI" lnInst="1" doName="Beh" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_4U8_" lnClass="XSWI" lnInst="1" doName="Health" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="SwTyp" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Loc" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="OpCnt" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Pos" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Beh" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Health" fc="ST" />
            </DataSet>
            <ReportControl rptID="PATIO_4U8LD0/PATIO_4U8_XSWI1$RP$SETReportCB_1" confRev="10000" buffered="false" indexed="true" name="SETReportCB_1" datSet="SETReportDS_1" desc="">
              <OptFields />
              <RptEnabled max="1" desc="">
                <ClientLN desc="" iedName="U_CENTRAL" ldInst="LD0" prefix="" lnClass="LLN0" lnInst="" />
              </RptEnabled>
            </ReportControl>
            <Inputs>
              <ExtRef desc="" iedName="UAC_4U_P" ldInst="LD0" prefix="CP_CO_4U8_" lnClass="CSWI" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="UAC_4U_S" ldInst="LD0" prefix="CS_CO_4U8_" lnClass="CSWI" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
          <LN prefix="PATIO_" lnClass="XSWI" inst="1" lnType="XSWI_CEMIG">
            <Inputs>
              <ExtRef desc="" iedName="UAC_4U_P" ldInst="LD0" prefix="CP_CO_4U8T_" lnClass="CSWI" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="UAC_4U_S" ldInst="LD0" prefix="CS_CO_4U8T_" lnClass="CSWI" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
        </LDevice>
      </Server>
    </AccessPoint>
  </IED>
  <IED name="PATIO_4U5" desc="">
    <Private type="SchneiderElectric-SET-UserInterface">
      <MetaInfosPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <ElectricalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000001">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="0" Height="0" />
              <Position X="0" Y="0" />
            </GraphicalProperties>
          </WorkArea>
        </ElectricalWorkAreas>
        <SystemWorkAreas>
          <WorkArea WorkAreaIdentification="da9b4ce3-826f-4bd4-a5cc-55623ea380fc">
            <GraphicalProperties DuplicateCount="1" IsPlaced="False" IsExpanded="False">
              <Size Width="50" Height="50" />
              <Position X="200" Y="50" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="05654ba5-3902-482b-9432-5fce4e2c1635">
            <GraphicalProperties DuplicateCount="1" IsPlaced="False" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="318" Y="507" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="31e25233-963c-4dc6-b5f2-82cf3fbc3e9f">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="129" Y="308" />
            </GraphicalProperties>
          </WorkArea>
          <WorkArea WorkAreaIdentification="5570cec2-94b6-41fa-bf42-9026ab8f5b26">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="False">
              <Size Width="35" Height="35" />
              <Position X="408.7" Y="342.62890625" />
            </GraphicalProperties>
          </WorkArea>
        </SystemWorkAreas>
        <LogicalWorkAreas>
          <WorkArea WorkAreaIdentification="00000000-0000-0000-0000-000000000003">
            <GraphicalProperties DuplicateCount="1" IsPlaced="True" IsExpanded="True">
              <Size Width="134.15" Height="40" />
              <Position X="106.1875" Y="870" />
            </GraphicalProperties>
          </WorkArea>
        </LogicalWorkAreas>
      </MetaInfosPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-ConsumerOrder">
      <ConsumerOrderPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates">
        <IEDNameOrder>
          <Item>
            <Key>
              <String>UAC_4U_P</String>
            </Key>
            <Value>
              <Int32>0</Int32>
            </Value>
          </Item>
          <Item>
            <Key>
              <String>UAC_4U_S</String>
            </Key>
            <Value>
              <Int32>1</Int32>
            </Value>
          </Item>
        </IEDNameOrder>
      </ConsumerOrderPrivates>
    </Private>
    <Private type="SchneiderElectric-SET-IEDSubscribeEditorFilter">
      <IEDSubscribeEditorFilterPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" MappedLNChecked="False" PointToPointChecked="False" BufferMXChecked="False" BufferSTChecked="False" IsAutomaticQualityClient="False" />
    </Private>
    <Private type="SchneiderElectric-SET-IEDProperties">
      <IEDPropertiesPrivates xmlns="http://www.schneider-electric.com/TSF/CBS/Privates" ICDType="" IIDCIDType="" IEDClass="" ICDRevision="-1" SetRevision="0" IsInError="False" ElementType="IED" ElementFunction="None" />
    </Private>
    <Services>
      <ConfDataSet modify="true" maxAttributes="100" max="100" />
      <ConfReportControl bufMode="both" max="64" />
      <GOOSE max="64" />
      <ConfLNs fixPrefix="false" fixLnInst="false" />
      <ConfLdName />
    </Services>
    <AccessPoint name="AP1" clock="false">
      <Server>
        <Authentication />
        <LDevice inst="LD0">
          <LN0 lnClass="LLN0" inst="" lnType="SE_LLN0_SET_default_V001">
            <DataSet name="SETGooseDS_1" desc="">
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Pos" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Health" daName="stVal" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Loc" daName="stVal" fc="ST" />
            </DataSet>
            <GSEControl type="GOOSE" appID="PATIO_4U5LD0/LLN0$GO$SETGooseCB_1" confRev="10000" name="SETGooseCB_1" datSet="SETGooseDS_1">
              <IEDName>UAC_4U_P</IEDName>
              <IEDName>UAC_4U_S</IEDName>
            </GSEControl>
          </LN0>
          <LN prefix="PATIO_" lnClass="XSWI" inst="1" lnType="XSWI_CEMIG">
            <DataSet name="SETReportDS_1" desc="">
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="SwTyp" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Loc" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="OpCnt" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Pos" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Beh" fc="ST" />
              <FCDA ldInst="LD0" prefix="PATIO_" lnClass="XSWI" lnInst="1" doName="Health" fc="ST" />
            </DataSet>
            <ReportControl rptID="PATIO_4U5LD0/PATIO_XSWI1$RP$SETReportCB_1" confRev="10000" buffered="false" indexed="true" name="SETReportCB_1" datSet="SETReportDS_1" desc="">
              <OptFields />
              <RptEnabled max="1" desc="">
                <ClientLN desc="" iedName="U_CENTRAL" ldInst="LD0" prefix="" lnClass="LLN0" lnInst="" />
              </RptEnabled>
            </ReportControl>
            <Inputs>
              <ExtRef desc="" iedName="UAC_4U_P" ldInst="LD0" prefix="CP_CO_4U5_" lnClass="CSWI" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
              <ExtRef desc="" iedName="UAC_4U_S" ldInst="LD0" prefix="CS_CO_4U5_" lnClass="CSWI" lnInst="1" doName="Pos" daName="stVal" serviceType="GOOSE" srcLDInst="LD0" srcLNClass="LLN0" srcCBName="SETGooseCB_1" />
            </Inputs>
          </LN>
        </LDevice>
      </Server>
    </AccessPoint>
  </IED>
  <DataTypeTemplates>
    <LNodeType iedType="SE_Spec_SET" lnClass="YEFN" id="SE_YEFN_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="LLN0" id="SE_LLN0_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="CILO" id="CILO_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="EnaOpn" type="SE_SPS_V001" desc="Enable open" />
      <DO name="EnaCls" type="SE_SPS_V001" desc="enabled close" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="RPSB" id="SE_RPSB_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="CSWI" id="SE_CSWI_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Pos" type="SE_DPC_V001" desc="Position" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="TCTR" id="SE_TCTR_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="ARtg" type="SE_ASG_V001" desc="Phase A (L1) Rating" />
      <DO name="HzRtg" type="SE_ASG_V001" desc="Frequency Rating" />
      <DO name="Clip" type="SE_ASG_V001" desc="Clip" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="RSYN" id="SE_RSYN_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="PTRC" id="SE_PTRC_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="RBRF" id="SE_RBRF_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Beh" type="SE_ENS_V001" desc="" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="RREC" id="SE_RREC_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="OpCls" type="SE_ACT_V001" desc="operate close" />
      <DO name="AutoRecSt" type="SE_ENS_V002" desc="Automatic Reclose status" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="PSOF" id="PSOF_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Op" type="SE_ACT_V001" desc="operate" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="PSCH" id="SE_PSCH_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Op" type="SE_ACT_V001" desc="operate" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="ZREA" id="SE_ZREA_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="PDIS" id="SE_PDIS_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Str" type="SE_ACD_V001" desc="Start" />
      <DO name="Op" type="SE_ACT_V001" desc="operate" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="PTOC" id="SE_PTOC_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Str" type="SE_ACD_V001" desc="Start" />
      <DO name="Op" type="SE_ACT_V001" desc="operate" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="XCBR" id="XCBR_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Loc" type="SE_SPS_V001" />
      <DO name="OpCnt" type="SE_INS_V001" desc="operate Counter" />
      <DO name="Pos" type="SE_DPC_V001" desc="Position" />
      <DO name="BlkOpn" type="SE_SPC_V001" desc="Blocked open" />
      <DO name="BlkCls" type="SE_SPC_V001" desc="Blocked close" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
      <DO name="CBOpCap" type="SE_ENS_V005" desc="Circuit breaker operate capability" />
      <DO name="Dsc" type="SE_SPS_V001" desc="Discrepancy" />
      <DO name="ChaMotEna" type="SE_SPC_V001" desc="Charger Motor enabled" />
      <DO name="Health" type="SE_ENS_V004" desc="Health" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="PIOC" id="PIOC_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Op" type="SE_ACT_V001" desc="operate" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="RREC" id="RREC_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="OpCls" type="SE_ACT_V001" desc="operate close" />
      <DO name="AutoRecSt" type="SE_ENS_V002" desc="Automatic Reclose status" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
      <DO name="Blk" type="SE_SPS_V001" desc="block" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="PTOV" id="SE_PTOV_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Str" type="SE_ACD_V001" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="PIOC" id="SE_PIOC_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Op" type="SE_ACT_V001" desc="operate" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="RADR" id="SE_RADR_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="RDRE" id="RDRE_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="RcdMade" type="SE_SPS_V001" desc="record Made" />
      <DO name="FltNum" type="SE_INS_V001" desc="Fault Number" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
      <DO name="Health" type="SE_ENS_V004" desc="Health" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="ZLIN" id="SE_ZLIN_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="PTOC" id="PTOC_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Str" type="SE_ACD_V001" desc="Start" />
      <DO name="Op" type="SE_ACT_V001" desc="operate" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="RPSB" id="RPSB_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
      <DO name="Op" type="SE_ACT_V001" desc="operate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="PSOF" id="SE_PSOF_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Op" type="SE_ACT_V001" desc="operate" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="TVTR" id="SE_TVTR_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="VRtg" type="SE_ASG_V001" desc="Voltage Rating" />
      <DO name="HzRtg" type="SE_ASG_V001" desc="Frequency Rating" />
      <DO name="Clip" type="SE_ASG_V001" desc="Clip" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="PPAM" id="PPAM_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Str" type="SE_ACD_V001" />
      <DO name="Op" type="SE_ACT_V001" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="CALH" id="SE_CALH_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="PDIS" id="PDIS_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Str" type="SE_ACD_V001" desc="Start" />
      <DO name="Op" type="SE_ACT_V002" desc="operate" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="ITCI" id="SE_ITCI_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="RBRF" id="RBRF_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
      <DO name="Str" type="SE_ACD_V001" desc="Start" />
      <DO name="OpEx" type="SE_ACT_V001" desc="operate External" />
      <DO name="OpIn" type="SE_ACT_V001" desc="operate Input" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="CALH" id="CALH_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
      <DO name="GrAlm" type="SE_SPS_V001" desc="Group Alarm" />
      <DO name="GrInd" type="SE_SPS_V001" desc="Group Indication" />
      <DO name="GrWrn" type="SE_SPS_V001" desc="Group Warning" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="XSWI" id="SE_XSWI_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="SwTyp" type="SE_ENS_V003" desc="Switched Type" />
      <DO name="Loc" type="SE_SPS_V001" />
      <DO name="OpCnt" type="SE_INS_V001" desc="operate Counter" />
      <DO name="Pos" type="SE_DPC_V001" desc="Position" />
      <DO name="BlkOpn" type="SE_SPC_V001" desc="Blocked open" />
      <DO name="BlkCls" type="SE_SPC_V001" desc="Blocked close" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="CILO" id="SE_CILO_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="EnaOpn" type="SE_SPS_V001" desc="Enable open" />
      <DO name="EnaCls" type="SE_SPS_V001" desc="enabled close" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="PTRC" id="PTRC_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="Op" type="SE_ACT_V003" desc="operate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="PTOV" id="PTOV_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Str" type="SE_ACD_V001" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="Op" type="SE_ACT_V001" desc="operate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="RBDR" id="SE_RBDR_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="PPAM" id="SE_PPAM_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Str" type="SE_ACD_V001" />
      <DO name="Op" type="SE_ACT_V001" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="RSYN" id="RSYN_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="Rel" type="SE_SPS_V001" desc="Release" />
      <DO name="VInd" type="SE_SPS_V001" desc="Voltage Indication" />
      <DO name="AngInd" type="SE_SPS_V001" desc="Angle Indication" />
      <DO name="HzInd" type="SE_SPS_V001" desc="Frequency Indication" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="RBDR" id="RBDR_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
      <DO name="Health" type="SE_ENS_V004" desc="Health" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="PSCH" id="PSCH_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Op" type="SE_ACT_V001" desc="operate" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
      <DO name="RxTr1" type="SE_ACT_V001" desc="" />
      <DO name="RxPrm1" type="SE_ACT_V001" desc="" />
      <DO name="TxTr" type="SE_ACT_V001" desc="transmit Trip (electrical protection function)" />
      <DO name="TxPrm" type="SE_ACT_V001" desc="transmit Permissive" />
      <DO name="RxPrm2" type="SE_ACT_V001" desc="receive Permissive" />
      <DO name="RxTr2" type="SE_ACT_V001" desc="Received Trip (electrical protection function)" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="MMXU" id="MMXU_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
      <DO name="VA" type="SE_WYE_V001" desc="Apparent power (volt amperes)" />
      <DO name="VAr" type="SE_WYE_V001" desc="Reactive power (volt amperes reactive)" />
      <DO name="W" type="SE_WYE_V001" desc="Active power" />
      <DO name="A" type="SE_WYE_V001" desc="current" />
      <DO name="PPV" type="SE_DEL_V001" desc="Phase to phase voltage" />
      <DO name="Health" type="SE_ENS_V004" desc="Health" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="RDRE" id="SE_RDRE_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="RcdMade" type="SE_SPS_V001" desc="record Made" />
      <DO name="FltNum" type="SE_INS_V001" desc="Fault Number" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="MMXU" id="SE_MMXU_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="IHMI" id="SE_IHMI_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="CSWI" id="CSWI_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Pos" type="SE_DPC_V001" desc="Position" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="XSWI" id="XSWI_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="SwTyp" type="SE_ENS_V003" desc="Switched Type" />
      <DO name="Loc" type="SE_SPS_V001" />
      <DO name="OpCnt" type="SE_INS_V001" desc="operate Counter" />
      <DO name="Pos" type="SE_DPC_V001" desc="Position" />
      <DO name="BlkOpn" type="SE_SPC_V001" desc="Blocked open" />
      <DO name="BlkCls" type="SE_SPC_V001" desc="Blocked close" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
      <DO name="Health" type="SE_ENS_V004" desc="Health" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="XCBR" id="SE_XCBR_SET_default_V001">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Loc" type="SE_SPS_V001" />
      <DO name="OpCnt" type="SE_INS_V001" desc="operate Counter" />
      <DO name="Pos" type="SE_DPC_V001" desc="Position" />
      <DO name="BlkOpn" type="SE_SPC_V001" desc="Blocked open" />
      <DO name="BlkCls" type="SE_SPC_V001" desc="Blocked close" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
    </LNodeType>
    <LNodeType iedType="SE_Spec_SET" lnClass="RADR" id="RADR_CEMIG">
      <Private type="CBSIsSpecDefinitionType" />
      <DO name="Beh" type="SE_ENS_V001" desc="Behaviour" />
      <DO name="NamPlt" type="SE_LPL_V001" desc="Name plate" />
      <DO name="Health" type="SE_ENS_V004" desc="Health" />
    </LNodeType>
    <DOType cdc="LPL" id="SE_LPL_V001">
      <DA dchg="false" qchg="false" dupd="false" fc="EX" name="lnNs" bType="VisString255" desc="logical node name space">
        <Val>IEC 61850-7-4:2007B</Val>
      </DA>
    </DOType>
    <DOType cdc="ASG" id="SE_ASG_V001">
      <DA dchg="true" qchg="false" dupd="false" fc="SP" name="setMag" bType="Struct" type="SE_setMag_V001" />
    </DOType>
    <DOType cdc="ENS" id="SE_ENS_V001">
      <DA dchg="true" qchg="false" dupd="true" fc="ST" name="stVal" bType="Enum" type="SE_stVal_V001" desc="state value" />
      <DA dchg="false" qchg="true" dupd="false" fc="ST" name="q" bType="Quality" desc="quality" />
      <DA dchg="false" qchg="false" dupd="false" fc="ST" name="t" bType="Timestamp" desc="time" />
    </DOType>
    <DOType cdc="ENS" id="SE_ENS_V003">
      <DA dchg="true" qchg="false" dupd="true" fc="ST" name="stVal" bType="Enum" type="SE_stVal_V003" desc="state value" />
      <DA dchg="false" qchg="true" dupd="false" fc="ST" name="q" bType="Quality" desc="quality" />
      <DA dchg="false" qchg="false" dupd="false" fc="ST" name="t" bType="Timestamp" desc="time" />
    </DOType>
    <DOType cdc="DEL" id="SE_DEL_V001" />
    <DOType cdc="ENS" id="SE_ENS_V005">
      <DA dchg="true" qchg="false" dupd="true" fc="ST" name="stVal" bType="Enum" type="SE_stVal_V005" desc="state value" />
      <DA dchg="false" qchg="true" dupd="false" fc="ST" name="q" bType="Quality" desc="quality" />
      <DA dchg="false" qchg="false" dupd="false" fc="ST" name="t" bType="Timestamp" desc="time" />
    </DOType>
    <DOType cdc="ENS" id="SE_ENS_V002">
      <DA dchg="true" qchg="false" dupd="true" fc="ST" name="stVal" bType="Enum" type="SE_stVal_V002" desc="state value" />
      <DA dchg="false" qchg="true" dupd="false" fc="ST" name="q" bType="Quality" desc="quality" />
      <DA dchg="false" qchg="false" dupd="false" fc="ST" name="t" bType="Timestamp" desc="time" />
    </DOType>
    <DOType cdc="ENS" id="SE_ENS_V004">
      <DA dchg="true" qchg="false" dupd="true" fc="ST" name="stVal" bType="Enum" type="SE_stVal_V004" desc="state value" />
      <DA dchg="false" qchg="true" dupd="false" fc="ST" name="q" bType="Quality" desc="quality" />
      <DA dchg="false" qchg="false" dupd="false" fc="ST" name="t" bType="Timestamp" desc="time" />
    </DOType>
    <DOType cdc="SPC" id="SE_SPC_V001">
      <DA dchg="true" qchg="false" dupd="false" fc="CF" name="ctlModel" bType="Enum" type="SE_ctlModel_V001" desc="control model" />
    </DOType>
    <DOType cdc="ACD" id="SE_ACD_V001">
      <DA dchg="true" qchg="false" dupd="false" fc="ST" name="general" bType="BOOLEAN" desc="general" />
      <DA dchg="true" qchg="false" dupd="false" fc="ST" name="dirGeneral" bType="Enum" type="SE_dirGeneral_V001" desc="direction general" />
      <DA dchg="false" qchg="true" dupd="false" fc="ST" name="q" bType="Quality" desc="quality" />
      <DA dchg="false" qchg="false" dupd="false" fc="ST" name="t" bType="Timestamp" desc="time" />
    </DOType>
    <DOType cdc="SPS" id="SE_SPS_V001">
      <DA dchg="true" qchg="false" dupd="false" fc="ST" name="stVal" bType="BOOLEAN" desc="state value" />
      <DA dchg="false" qchg="true" dupd="false" fc="ST" name="q" bType="Quality" desc="quality" />
      <DA dchg="false" qchg="false" dupd="false" fc="ST" name="t" bType="Timestamp" desc="time" />
    </DOType>
    <DOType cdc="INS" id="SE_INS_V001">
      <DA dchg="true" qchg="false" dupd="true" fc="ST" name="stVal" bType="INT32" desc="state value" />
      <DA dchg="false" qchg="true" dupd="false" fc="ST" name="q" bType="Quality" desc="quality" />
      <DA dchg="false" qchg="false" dupd="false" fc="ST" name="t" bType="Timestamp" desc="time" />
    </DOType>
    <DOType cdc="DPC" id="SE_DPC_V001">
      <DA dchg="true" qchg="false" dupd="false" fc="ST" name="stVal" bType="Dbpos" desc="state value" />
      <DA dchg="false" qchg="true" dupd="false" fc="ST" name="q" bType="Quality" desc="quality" />
      <DA dchg="false" qchg="false" dupd="false" fc="ST" name="t" bType="Timestamp" desc="time" />
      <DA dchg="true" qchg="false" dupd="false" fc="CF" name="ctlModel" bType="Enum" type="SE_ctlModel_V001" desc="control model" />
    </DOType>
    <DOType cdc="WYE" id="SE_WYE_V001" />
    <DOType cdc="ACT" id="SE_ACT_V003">
      <DA dchg="true" qchg="false" dupd="false" fc="ST" name="general" bType="BOOLEAN" desc="general" />
      <DA dchg="true" qchg="false" dupd="false" fc="ST" name="neut" bType="BOOLEAN" desc="neutral" />
      <DA dchg="true" qchg="false" dupd="false" fc="ST" name="phsC" bType="BOOLEAN" desc="&lt;p&gt;Value true indicates a command, a trip or a start event of phase C.&lt;/p&gt;" />
      <DA dchg="true" qchg="false" dupd="false" fc="ST" name="phsB" bType="BOOLEAN" desc="&lt;p&gt;Value true indicates a command, a trip or a start event of phase B.&lt;/p&gt;" />
      <DA dchg="true" qchg="false" dupd="false" fc="ST" name="phsA" bType="BOOLEAN" desc="&lt;p&gt;Value true indicates a command, a trip or a start event of phase A.&lt;/p&gt;" />
      <DA dchg="false" qchg="true" dupd="false" fc="ST" name="q" bType="Quality" desc="quality" />
      <DA dchg="false" qchg="false" dupd="false" fc="ST" name="t" bType="Timestamp" desc="time" />
    </DOType>
    <DOType cdc="ACT" id="SE_ACT_V002">
      <DA dchg="true" qchg="false" dupd="false" fc="ST" name="general" bType="BOOLEAN" desc="general" />
      <DA dchg="true" qchg="false" dupd="false" fc="ST" name="neut" bType="BOOLEAN" desc="neutral" />
      <DA dchg="true" qchg="false" dupd="false" fc="ST" name="phsC" bType="BOOLEAN" desc="&lt;p&gt;Value true indicates a command, a trip or a start event of phase C.&lt;/p&gt;" />
      <DA dchg="true" qchg="false" dupd="false" fc="ST" name="phsB" bType="BOOLEAN" desc="&lt;p&gt;Value true indicates a command, a trip or a start event of phase B.&lt;/p&gt;" />
      <DA dchg="true" qchg="false" dupd="false" fc="ST" name="phsA" bType="BOOLEAN" desc="&lt;p&gt;Value true indicates a command, a trip or a start event of phase A.&lt;/p&gt;" />
      <DA dchg="false" qchg="true" dupd="false" fc="ST" name="q" bType="Quality" desc="quality" />
      <DA dchg="false" qchg="false" dupd="false" fc="ST" name="t" bType="Timestamp" desc="time" />
    </DOType>
    <DOType cdc="ACT" id="SE_ACT_V001">
      <DA dchg="true" qchg="false" dupd="false" fc="ST" name="general" bType="BOOLEAN" desc="general" />
      <DA dchg="false" qchg="true" dupd="false" fc="ST" name="q" bType="Quality" desc="quality" />
      <DA dchg="false" qchg="false" dupd="false" fc="ST" name="t" bType="Timestamp" desc="time" />
    </DOType>
    <DAType id="SE_setMag_V001">
      <BDA name="i" bType="INT32" />
    </DAType>
    <EnumType id="SE_ctlModel_V001">
      <EnumVal ord="0">status-only</EnumVal>
      <EnumVal ord="1">direct-with-normal-security</EnumVal>
      <EnumVal ord="2">sbo-with-normal-security</EnumVal>
      <EnumVal ord="3">direct-with-enhanced-security</EnumVal>
      <EnumVal ord="4">sbo-with-enhanced-security</EnumVal>
    </EnumType>
    <EnumType id="SE_stVal_V002">
      <EnumVal ord="1">Ready</EnumVal>
      <EnumVal ord="2">InProgress</EnumVal>
      <EnumVal ord="3">Successful</EnumVal>
      <EnumVal ord="4">WaitingForTrip</EnumVal>
      <EnumVal ord="5">TripFromProtection</EnumVal>
      <EnumVal ord="6">FaultDisappeared</EnumVal>
      <EnumVal ord="7">WaitToComplete</EnumVal>
      <EnumVal ord="8">CBclosed</EnumVal>
      <EnumVal ord="9">CycleUnsuccessful</EnumVal>
      <EnumVal ord="10">Unsuccessful</EnumVal>
      <EnumVal ord="11">Aborted</EnumVal>
    </EnumType>
    <EnumType id="SE_dirGeneral_V001">
      <EnumVal ord="0">unknown</EnumVal>
      <EnumVal ord="1">forward</EnumVal>
      <EnumVal ord="2">backward</EnumVal>
      <EnumVal ord="3">both</EnumVal>
    </EnumType>
    <EnumType id="SE_stVal_V003">
      <EnumVal ord="1">Load Break</EnumVal>
      <EnumVal ord="2">Disconnector</EnumVal>
      <EnumVal ord="3">Earthing Switch</EnumVal>
      <EnumVal ord="4">High Speed Earthing Switch</EnumVal>
    </EnumType>
    <EnumType id="SE_stVal_V001">
      <EnumVal ord="1">on</EnumVal>
      <EnumVal ord="2">blocked</EnumVal>
      <EnumVal ord="3">test</EnumVal>
      <EnumVal ord="4">test/blocked</EnumVal>
      <EnumVal ord="5">off</EnumVal>
    </EnumType>
    <EnumType id="SE_stVal_V005">
      <EnumVal ord="1">None</EnumVal>
      <EnumVal ord="2">Open</EnumVal>
      <EnumVal ord="3">Close-Open</EnumVal>
      <EnumVal ord="4">Open-Close-Open</EnumVal>
      <EnumVal ord="5">Close-Open-Close-Open</EnumVal>
      <EnumVal ord="6">Open-Close-Open-Close-Open</EnumVal>
      <EnumVal ord="7">more</EnumVal>
    </EnumType>
    <EnumType id="SE_stVal_V004">
      <EnumVal ord="1">Ok</EnumVal>
      <EnumVal ord="2">Warning</EnumVal>
      <EnumVal ord="3">Alarm</EnumVal>
    </EnumType>
  </DataTypeTemplates>
</SCL>