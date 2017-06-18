Partial Class FormClassCreate
    Inherits System.Windows.Forms.Form


    Private Const MAX_KEY_LENGTH As Integer = 6
    Friend WithEvents MenuStrip1 As MenuStrip
    Friend WithEvents FileToolStripMenuItem As ToolStripMenuItem
    Friend WithEvents ButtonToolStripMenuItem As ToolStripMenuItem
    Friend WithEvents txtCaption As TextBox
    Friend WithEvents txtKey As TextBox
    Friend WithEvents lblCaption As Label
    Friend WithEvents lblKey As Label
    Friend WithEvents mnuShow As ToolStripMenuItem
    Friend WithEvents mnuExit As ToolStripMenuItem
    Friend WithEvents mnuCreate As ToolStripMenuItem
    Friend WithEvents mnuRemove As ToolStripMenuItem
    Private _frmShow As FormClassShow

    Public Sub New()
        ' Need to call InitializeComponent definded on Desinger.vb 
        InitializeComponent()

        _frmShow = New FormClassShow
        _frmShow.MainForm = Me

    End Sub

    Private Sub InitializeComponent()
        Me.MenuStrip1 = New System.Windows.Forms.MenuStrip()
        Me.FileToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.mnuShow = New System.Windows.Forms.ToolStripMenuItem()
        Me.mnuExit = New System.Windows.Forms.ToolStripMenuItem()
        Me.ButtonToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.mnuCreate = New System.Windows.Forms.ToolStripMenuItem()
        Me.mnuRemove = New System.Windows.Forms.ToolStripMenuItem()
        Me.txtCaption = New System.Windows.Forms.TextBox()
        Me.txtKey = New System.Windows.Forms.TextBox()
        Me.lblCaption = New System.Windows.Forms.Label()
        Me.lblKey = New System.Windows.Forms.Label()
        Me.MenuStrip1.SuspendLayout()
        Me.SuspendLayout()
        '
        'MenuStrip1
        '
        Me.MenuStrip1.ImageScalingSize = New System.Drawing.Size(20, 20)
        Me.MenuStrip1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.FileToolStripMenuItem, Me.ButtonToolStripMenuItem})
        Me.MenuStrip1.Location = New System.Drawing.Point(0, 0)
        Me.MenuStrip1.Name = "MenuStrip1"
        Me.MenuStrip1.Size = New System.Drawing.Size(514, 28)
        Me.MenuStrip1.TabIndex = 0
        Me.MenuStrip1.Text = "MenuStrip1"
        '
        'FileToolStripMenuItem
        '
        Me.FileToolStripMenuItem.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.mnuShow, Me.mnuExit})
        Me.FileToolStripMenuItem.Name = "FileToolStripMenuItem"
        Me.FileToolStripMenuItem.Size = New System.Drawing.Size(44, 24)
        Me.FileToolStripMenuItem.Text = "&File"
        '
        'mnuShow
        '
        Me.mnuShow.Name = "mnuShow"
        Me.mnuShow.ShortcutKeys = CType((System.Windows.Forms.Keys.Control Or System.Windows.Forms.Keys.S), System.Windows.Forms.Keys)
        Me.mnuShow.Size = New System.Drawing.Size(181, 26)
        Me.mnuShow.Text = "&Show"
        '
        'mnuExit
        '
        Me.mnuExit.Name = "mnuExit"
        Me.mnuExit.ShortcutKeys = CType((System.Windows.Forms.Keys.Control Or System.Windows.Forms.Keys.X), System.Windows.Forms.Keys)
        Me.mnuExit.Size = New System.Drawing.Size(181, 26)
        Me.mnuExit.Text = "E&xit"
        '
        'ButtonToolStripMenuItem
        '
        Me.ButtonToolStripMenuItem.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.mnuCreate, Me.mnuRemove})
        Me.ButtonToolStripMenuItem.Name = "ButtonToolStripMenuItem"
        Me.ButtonToolStripMenuItem.Size = New System.Drawing.Size(65, 24)
        Me.ButtonToolStripMenuItem.Text = "&Button"
        '
        'mnuCreate
        '
        Me.mnuCreate.Name = "mnuCreate"
        Me.mnuCreate.Size = New System.Drawing.Size(181, 26)
        Me.mnuCreate.Text = "&Create"
        '
        'mnuRemove
        '
        Me.mnuRemove.Name = "mnuRemove"
        Me.mnuRemove.Size = New System.Drawing.Size(181, 26)
        Me.mnuRemove.Text = "&Remove"
        '
        'txtCaption
        '
        Me.txtCaption.Location = New System.Drawing.Point(306, 176)
        Me.txtCaption.Name = "txtCaption"
        Me.txtCaption.Size = New System.Drawing.Size(100, 22)
        Me.txtCaption.TabIndex = 1
        Me.txtCaption.Text = "First"
        '
        'txtKey
        '
        Me.txtKey.Location = New System.Drawing.Point(112, 176)
        Me.txtKey.Name = "txtKey"
        Me.txtKey.Size = New System.Drawing.Size(100, 22)
        Me.txtKey.TabIndex = 2
        Me.txtKey.Text = "S17"
        '
        'lblCaption
        '
        Me.lblCaption.AutoSize = True
        Me.lblCaption.Location = New System.Drawing.Point(303, 156)
        Me.lblCaption.Name = "lblCaption"
        Me.lblCaption.Size = New System.Drawing.Size(101, 17)
        Me.lblCaption.TabIndex = 3
        Me.lblCaption.Text = "Button Caption"
        '
        'lblKey
        '
        Me.lblKey.AutoSize = True
        Me.lblKey.Location = New System.Drawing.Point(109, 156)
        Me.lblKey.Name = "lblKey"
        Me.lblKey.Size = New System.Drawing.Size(77, 17)
        Me.lblKey.TabIndex = 4
        Me.lblKey.Text = "Button Key"
        '
        'FormClassCreate
        '
        Me.ClientSize = New System.Drawing.Size(514, 354)
        Me.ControlBox = False
        Me.Controls.Add(Me.lblKey)
        Me.Controls.Add(Me.lblCaption)
        Me.Controls.Add(Me.txtKey)
        Me.Controls.Add(Me.txtCaption)
        Me.Controls.Add(Me.MenuStrip1)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D
        Me.MainMenuStrip = Me.MenuStrip1
        Me.Name = "FormClassCreate"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "Program 1 - Grant Oberhauser"
        Me.MenuStrip1.ResumeLayout(False)
        Me.MenuStrip1.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Private Sub FormClassCreate_FormClosing(sender As Object, e As FormClosingEventArgs) Handles MyBase.FormClosing
        If e.CloseReason = CloseReason.UserClosing Then
            e.Cancel = True
        End If
    End Sub

    Private Sub mnuCreate_Click(sender As Object, e As EventArgs) Handles mnuCreate.Click

        Try
            _frmShow.addButton(txtKey.Text, txtCaption.Text)
        Catch ex As Exception

        End Try

    End Sub

    Private Sub txtKey_KeyPress(sender As Object, e As KeyPressEventArgs) Handles txtKey.KeyPress

        Dim keyString As String = txtKey.Text
        Dim valid As Boolean = True


        If keyString.Length > MAX_KEY_LENGTH Then
            valid = False
        ElseIf keyString.Contains(" ") Then
            valid = False
        ElseIf IsNumeric(keyString.Substring(0, 1)) Then
            valid = False
        ElseIf keyString.Length = 0 Then

            valid = False

            MessageBox.Show(Me, "Must provide key!", "Prog1")

        End If

    End Sub

    Private Sub mnuShow_Click(sender As Object, e As EventArgs) Handles mnuShow.Click

        Me.Hide()

        _frmShow.Show()


    End Sub

    Private Sub mnuExit_Click(sender As Object, e As EventArgs) Handles mnuExit.Click

        Application.Exit()

    End Sub
End Class