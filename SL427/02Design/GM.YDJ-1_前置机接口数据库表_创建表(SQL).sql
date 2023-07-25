
SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER ON
SET ANSI_PADDING ON

IF NOT EXISTS (SELECT name FROM sysObjects WHERE name='FLOWV_DATA' AND  type='u')
BEGIN
	CREATE TABLE [dbo].[FLOWV_DATA]
	(
		[P_ID] [decimal](18, 0) IDENTITY(1,1) NOT NULL,
		[RTU] [varchar](16) NOT NULL,
		[SENSORS_NO] [varchar](50) NOT NULL,
		[TOTAL_WATER] [numeric](8, 0) NOT NULL,
		[ANYTIME_FLOW] [numeric](4, 0) NOT NULL,
		[COLLECT_TIME] [datetime] NOT NULL,
		[SAVE_TIME] [datetime] NOT NULL,
		CONSTRAINT [PK_FLOWV_DATA] PRIMARY KEY CLUSTERED ([P_ID] ASC)
	)
END

IF NOT EXISTS (SELECT name FROM sysobjects WHERE name='WATERQ_DATA' AND type='u')
BEGIN
	CREATE TABLE [dbo].[WATERQ_DATA]
	(
		[P_ID] [decimal](18, 0) IDENTITY(1,1) NOT NULL,
		[RTU] [varchar](16) NOT NULL,
		[SPECIES] [varchar](3) NOT NULL,
		[CURRENT_VAL] [varchar](50) NOT NULL,
		[COLLECT_TIME] [datetime] NOT NULL,
		[SAVE_TIME] [datetime] NOT NULL,
		CONSTRAINT [PK_WATERQ_DATA] PRIMARY KEY CLUSTERED ([P_ID] ASC)
	)
END

IF NOT EXISTS (SELECT name FROM sysobjects WHERE name='RAIN_DATA' AND type='u')
BEGIN
	CREATE TABLE [dbo].[RAIN_DATA]
	(
		[P_ID] [decimal](18, 0) IDENTITY(1,1) NOT NULL,
		[RTU] [varchar](16) NOT NULL,
		[SENSORS_NO] [varchar](10) NOT NULL,
		[DATA_TYPE] [varchar](10) NOT NULL,
		[CURRENT_VAL] [varchar](10) NOT NULL,
		[HALFH_VAL] [varchar](10) NOT NULL,
		[TOTAL_VAL] [varchar](10) NOT NULL,
		[COLLECT_TIME] [datetime] NOT NULL,
		[SAVE_TIME] [datetime] NOT NULL,
		CONSTRAINT [PK_RAIN_DATA] PRIMARY KEY CLUSTERED ([P_ID] ASC)
	)
END

IF NOT EXISTS (SELECT name FROM sysobjects WHERE name='ANALOG_DATA' AND type='u')
BEGIN
	CREATE TABLE [dbo].[ANALOG_DATA]
	(
		[P_ID] [decimal](18, 0) IDENTITY(1,1) NOT NULL,
		[RTU] [varchar](16) NOT NULL,
		[ANALOG_NO] [varchar](10) NOT NULL,
		[CURRENT_VAL] [varchar](10) NOT NULL,
		[COLLECT_TIME] [datetime] NOT NULL,
		[SAVE_TIME] [datetime] NOT NULL,
		CONSTRAINT [PK_ANALOG_DATA] PRIMARY KEY CLUSTERED ([P_ID] ASC)
	)
END

IF NOT EXISTS (SELECT name FROM sysobjects WHERE name='PULSE_DATA' AND type='u')
BEGIN
	CREATE TABLE [dbo].[PULSE_DATA]
	(
		[P_ID] [decimal](18, 0) IDENTITY(1,1) NOT NULL,
		[RTU] [varchar](16) NOT NULL,
		[CODE] [varchar](16) NOT NULL,
		[VALUE1] [varchar](10) NOT NULL,
		[VALUE2] [varchar](10) NOT NULL,
		[VALUE3] [varchar](10) NOT NULL,
		[VALUE4] [varchar](10) NOT NULL,
		[COLLECT_TIME] [datetime] NOT NULL,
		[SAVE_TIME] [datetime] NOT NULL,
		CONSTRAINT [PK_PULSE_DATA] PRIMARY KEY CLUSTERED ([P_ID] ASC)
	)
END

IF NOT EXISTS (SELECT name FROM sysobjects WHERE name='ALARM_RECORD' AND type='u')
BEGIN
	CREATE TABLE [dbo].[ALARM_RECORD]
	(
		[P_ID] [varchar](64) NOT NULL,
		[RTU] [varchar](16) NOT NULL,
		[W_TYPE] [int] NOT NULL,
		[WARNING_TIME] [datetime] NOT NULL,
		[SAVE_TIME] [datetime] NOT NULL,
		CONSTRAINT [PK_ALARM_RECORD] PRIMARY KEY CLUSTERED ([P_ID] ASC)
	)
END

IF NOT EXISTS (SELECT name FROM sysobjects WHERE name='ALARM_WQ' AND type='u')
BEGIN
	CREATE TABLE [dbo].[ALARM_WQ]
	(
		[P_ID] [decimal](18, 0) IDENTITY(1,1) NOT NULL,
		[WARNING_ID] [varchar](64) NOT NULL,
		[SPECIES] [varchar](3) NOT NULL,
		[CURRENT_VAL] [varchar](10) NOT NULL,
		CONSTRAINT [PK_ALARM_WQ] PRIMARY KEY CLUSTERED ([P_ID] ASC)
	)
END

IF NOT EXISTS (SELECT name FROM sysobjects WHERE name='ALARM_WL' AND type='u')
BEGIN
	CREATE TABLE [dbo].[ALARM_WL]
	(
		[P_ID] [decimal](18, 0) IDENTITY(1,1) NOT NULL,
		[WARNING_ID] [varchar](64) NOT NULL,
		[SENSORS_NO] [varchar](10) NOT NULL,
		[CURRENT_VAL] [varchar](10) NOT NULL,
		CONSTRAINT [PK_ALARM_WL] PRIMARY KEY CLUSTERED ([P_ID] ASC)
	)
END

IF NOT EXISTS (SELECT name FROM sysobjects WHERE name='ALARM_WP' AND type='u')
BEGIN
	CREATE TABLE [dbo].[ALARM_WP]
	(
		[P_ID] [decimal](18, 0) IDENTITY(1,1) NOT NULL,
		[WARNING_ID] [varchar](64) NOT NULL,
		[SENSORS_NO] [varchar](10) NOT NULL,
		[CURRENT_VAL] [varchar](10) NOT NULL,
		CONSTRAINT [PK_ALARM_WP] PRIMARY KEY CLUSTERED ([P_ID] ASC)
	)
END

IF NOT EXISTS (SELECT name FROM sysobjects WHERE name='ALARM_FL' AND type='u')
BEGIN
	CREATE TABLE [dbo].[ALARM_FL]
	(
		[P_ID] [decimal](18, 0) IDENTITY(1,1) NOT NULL,
		[WARNING_ID] [varchar](64) NOT NULL,
		[SENSORS_NO] [varchar](10) NOT NULL,
		[TOTAL_WATER] [numeric](8, 0) NOT NULL,
		[ANYTIME_FLOW] [numeric](4, 0) NOT NULL,
		CONSTRAINT [PK_ALARM_FL] PRIMARY KEY CLUSTERED ([P_ID] ASC)
	)
END

IF NOT EXISTS (SELECT name FROM sysobjects WHERE name='ALARM_RAIN' AND type='u')
BEGIN
	CREATE TABLE [dbo].[ALARM_RAIN]
	(
		[P_ID] [decimal](18, 0) IDENTITY(1,1) NOT NULL,
		[WARNING_ID] [varchar](64) NOT NULL,
		[SENSORS_NO] [varchar](10) NOT NULL,
		[CURRENT_VAL] [varchar](10) NOT NULL,
		[PER_TIME] [numeric](4, 0) NOT NULL,
		CONSTRAINT [PK_ALARM_RAIN] PRIMARY KEY CLUSTERED ([P_ID] ASC)
	)
END 

IF NOT EXISTS (SELECT name FROM sysobjects WHERE name='ALARM_ANALOG' AND type='u')
BEGIN
	CREATE TABLE [dbo].[ALARM_ANALOG]
	(
		[P_ID] [decimal](18, 0) IDENTITY(1,1) NOT NULL,
		[WARNING_ID] [varchar](64) NOT NULL,
		[ANALOG_NO] [varchar](10) NOT NULL,
		[CURRENT_VAL] [varchar](10) NOT NULL,
		CONSTRAINT [PK_ALARM_ANALOG] PRIMARY KEY CLUSTERED ([P_ID] ASC)
	)
END

IF NOT EXISTS (SELECT name FROM sysobjects WHERE name='OPERATE_RECORD' AND type='u')
BEGIN
	CREATE TABLE [dbo].[OPERATE_RECORD]
	(
		[P_ID] [decimal](18, 0) IDENTITY(1,1) NOT NULL,
		[RTU] [varchar](16) NOT NULL,
		[OPERATE_ACCOUNT] [varchar](64) NOT NULL,
		[OPERATE_TIME] [datetime] NOT NULL,
		[OPERATE_DESC] [varchar](2000) NOT NULL,
		CONSTRAINT [PK_OPERATE_RECORD] PRIMARY KEY CLUSTERED ( [P_ID] ASC)
	)
END

IF NOT EXISTS (SELECT name FROM sysobjects WHERE name='WATERL_DATA' AND type='u')
BEGIN
	CREATE TABLE [dbo].[WATERL_DATA]
	(
		[P_ID] [decimal](18, 0) IDENTITY(1,1) NOT NULL,
		[RTU] [varchar](16) NOT NULL,
		[SENSORS_NO] [varchar](10) NOT NULL,
		[CURRENT_VAL] [varchar](10) NOT NULL,
		[COLLECT_TIME] [datetime] NOT NULL,
		[SAVE_TIME] [datetime] NOT NULL,
		CONSTRAINT [PK_WATERL_DATA] PRIMARY KEY CLUSTERED ( [P_ID] ASC )
	)
END

IF NOT EXISTS (SELECT name FROM sysobjects WHERE name='WATERP_DATA' AND type='u')
BEGIN
	CREATE TABLE [dbo].[WATERP_DATA]
	(
		[P_ID] [decimal](18, 0) IDENTITY(1,1) NOT NULL,
		[RTU] [varchar](16) NOT NULL,
		[SENSORS_NO] [varchar](10) NOT NULL,
		[CURRENT_VAL] [varchar](10) NOT NULL,
		[COLLECT_TIME] [datetime] NOT NULL,
		[SAVE_TIME] [datetime] NOT NULL,
		CONSTRAINT [PK_WATERP_DATA] PRIMARY KEY CLUSTERED ([P_ID] ASC )
	)
END

SET ANSI_NULLS OFF
SET QUOTED_IDENTIFIER OFF
SET ANSI_PADDING OFF

GO
