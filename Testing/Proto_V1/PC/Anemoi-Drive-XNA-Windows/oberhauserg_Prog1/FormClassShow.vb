Imports System.Security.AccessControl

Partial Class FormClassShow
    Inherits System.Windows.Forms.Form

    Private Const MAX_BUTTON_COUNT As Integer = 4

    ' For event resize
    Private Const BUTTON_HEIGHT As Integer = 23
    Private Const BUTTON_WIDTH As Integer = 75
    Private Const MENU_HEIGHT As Integer = 24
    Private Const MARGIN_SIZE As Integer = 100

    Private _frmMain As Form
    Friend WithEvents MenuStrip1 As MenuStrip
    Friend WithEvents FileToolStripMenuItem As ToolStripMenuItem
    Friend WithEvents mnuReturn As ToolStripMenuItem
    Friend WithEvents mnuExit As ToolStripMenuItem
    Friend WithEvents ToolStripMenuItem1 As ToolStripSeparator
    Friend WithEvents ContextMenuStrip1 As ContextMenuStrip
    Private components As System.ComponentModel.IContainer
    Friend WithEvents mnuBold As ToolStripMenuItem
    Friend WithEvents mnuItalic As ToolStripMenuItem
    Friend WithEvents mnuUnderline As ToolStripMenuItem
    Private _allButtons As New SortedList

    Private WithEvents aButton As Button

    ' Sets _frmMain to a form object
    Public WriteOnly Property MainForm As Form
        Set(ByVal value As Form)
            _frmMain = value
        End Set
    End Property

    Public Sub New()
        ' Need to call InitializeComponent definded on Desinger.vb 
        InitializeComponent()

    End Sub

    ' At most MAX_BUTTON_COUNT buttons can be created. If MAX_BUTTON_COUNT buttons have 
    ' been created already, then an exception will be thrown and no button will be added.
    ' If no button in _allButtons has theKey as its key, the method creates a button with theKey 
    ' and theCaption and adds the button to both the form and the sorted list _allButtons.
    ' The Add method of the sorted list will check the uniqueness condition on key and raise an
    ' exception if the condition is violated. The method does not check the condition by itself 
    ' and calls the Add method of the sorted list and lets the caller to handle the exception.
    ' No Try-and-Catch should be used inside the sub.
    Public Sub addButton(ByVal theKey As String, ByVal theCaption As String)

        If _allButtons.Count < MAX_BUTTON_COUNT Then

            aButton = New Button

            aButton.Text = theCaption
            aButton.Tag = theKey
            aButton.Height = BUTTON_HEIGHT
            aButton.Width = BUTTON_WIDTH

            _allButtons.Add(theKey, aButton)

            AddHandler aButton.Click, AddressOf ButtonClick

            ArrangeButtons()

            Me.Controls.Add(aButton)

        Else

            Throw New Exception()

        End If


    End Sub

    ' If a button in _allButtons has theKey as its key, the method removes the button from both 
    ' the form and the sorted list. Otherwise, it throws an exception.
    ' No Try-and-Catch should be used inside the sub.
    Public Sub removeButton(ByVal theKey As String)

        Dim _button As Button

        If Not _allButtons.Count <= 0 And _allButtons.ContainsKey(theKey) Then

            _button = _allButtons.GetKey(theKey)

            Controls.Remove(_button)

            _allButtons.Remove(theKey)



        Else

            Throw New Exception()

        End If

    End Sub

    ' Arranges all buttons on the form according to the requirements.
    ' It does not change any thing of any button, except its location on the form.
    ' You cannot use an if  (or select) statement inside the method!
    Private Sub ArrangeButtons()

        Dim centerHeight As Integer = Me.Height / 2
        Dim thisButton As Button

        For i As Integer = 0 To (_allButtons.Count - 1)

            thisButton = _allButtons.GetByIndex(i)



            thisButton.Location = New System.Drawing.Point((i + 1) * MARGIN_SIZE, centerHeight - BUTTON_HEIGHT / 2)

        Next

    End Sub


    ' Changes the font of all buttons when a shortcut menu item is clicked.
    ' It also updates the menu item as checked or unchecked.
    ' You cannot use an if  (or select) statement inside the method!
    Private Sub ChangeFont(ByVal f As FontStyle, ByVal m As ToolStripMenuItem)

    End Sub

    ' Sets the font style of button b according to whether the three menu items
    ' are checked or not.
    ' The method should be called after a new button is created.
    Private Sub setFont(ByVal b As Button)



    End Sub

    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Me.MenuStrip1 = New System.Windows.Forms.MenuStrip()
        Me.FileToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.mnuReturn = New System.Windows.Forms.ToolStripMenuItem()
        Me.mnuExit = New System.Windows.Forms.ToolStripMenuItem()
        Me.ToolStripMenuItem1 = New System.Windows.Forms.ToolStripSeparator()
        Me.ContextMenuStrip1 = New System.Windows.Forms.ContextMenuStrip(Me.components)
        Me.mnuBold = New System.Windows.Forms.ToolStripMenuItem()
        Me.mnuItalic = New System.Windows.Forms.ToolStripMenuItem()
        Me.mnuUnderline = New System.Windows.Forms.ToolStripMenuItem()
        Me.MenuStrip1.SuspendLayout()
        Me.ContextMenuStrip1.SuspendLayout()
        Me.SuspendLayout()
        '
        'MenuStrip1
        '
        Me.MenuStrip1.ImageScalingSize = New System.Drawing.Size(20, 20)
        Me.MenuStrip1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.FileToolStripMenuItem})
        Me.MenuStrip1.Location = New System.Drawing.Point(0, 0)
        Me.MenuStrip1.Name = "MenuStrip1"
        Me.MenuStrip1.Size = New System.Drawing.Size(632, 28)
        Me.MenuStrip1.TabIndex = 0
        Me.MenuStrip1.Text = "MenuStrip1"
        '
        'FileToolStripMenuItem
        '
        Me.FileToolStripMenuItem.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.mnuReturn, Me.ToolStripMenuItem1, Me.mnuExit})
        Me.FileToolStripMenuItem.Name = "FileToolStripMenuItem"
        Me.FileToolStripMenuItem.Size = New System.Drawing.Size(44, 24)
        Me.FileToolStripMenuItem.Text = "&File"
        '
        'mnuReturn
        '
        Me.mnuReturn.Name = "mnuReturn"
        Me.mnuReturn.ShortcutKeys = CType((System.Windows.Forms.Keys.Control Or System.Windows.Forms.Keys.R), System.Windows.Forms.Keys)
        Me.mnuReturn.Size = New System.Drawing.Size(181, 26)
        Me.mnuReturn.Text = "&Return"
        '
        'mnuExit
        '
        Me.mnuExit.Name = "mnuExit"
        Me.mnuExit.ShortcutKeys = CType((System.Windows.Forms.Keys.Control Or System.Windows.Forms.Keys.X), System.Windows.Forms.Keys)
        Me.mnuExit.Size = New System.Drawing.Size(181, 26)
        Me.mnuExit.Text = "E&xit"
        '
        'ToolStripMenuItem1
        '
        Me.ToolStripMenuItem1.Name = "ToolStripMenuItem1"
        Me.ToolStripMenuItem1.Size = New System.Drawing.Size(178, 6)
        '
        'ContextMenuStrip1
        '
        Me.ContextMenuStrip1.ImageScalingSize = New System.Drawing.Size(20, 20)
        Me.ContextMenuStrip1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.mnuBold, Me.mnuItalic, Me.mnuUnderline})
        Me.ContextMenuStrip1.Name = "ContextMenuStrip1"
        Me.ContextMenuStrip1.Size = New System.Drawing.Size(234, 82)
        '
        'mnuBold
        '
        Me.mnuBold.CheckOnClick = True
        Me.mnuBold.Name = "mnuBold"
        Me.mnuBold.ShortcutKeys = CType((System.Windows.Forms.Keys.Control Or System.Windows.Forms.Keys.B), System.Windows.Forms.Keys)
        Me.mnuBold.Size = New System.Drawing.Size(233, 26)
        Me.mnuBold.Text = "Font Bold"
        '
        'mnuItalic
        '
        Me.mnuItalic.CheckOnClick = True
        Me.mnuItalic.Name = "mnuItalic"
        Me.mnuItalic.ShortcutKeys = CType((System.Windows.Forms.Keys.Control Or System.Windows.Forms.Keys.I), System.Windows.Forms.Keys)
        Me.mnuItalic.Size = New System.Drawing.Size(233, 26)
        Me.mnuItalic.Text = "Font Italic"
        '
        'mnuUnderline
        '
        Me.mnuUnderline.CheckOnClick = True
        Me.mnuUnderline.Name = "mnuUnderline"
        Me.mnuUnderline.ShortcutKeys = CType((System.Windows.Forms.Keys.Control Or System.Windows.Forms.Keys.U), System.Windows.Forms.Keys)
        Me.mnuUnderline.Size = New System.Drawing.Size(233, 26)
        Me.mnuUnderline.Text = "Font Underline"
        '
        'FormClassShow
        '
        Me.ClientSize = New System.Drawing.Size(632, 342)
        Me.ControlBox = False
        Me.Controls.Add(Me.MenuStrip1)
        Me.MainMenuStrip = Me.MenuStrip1
        Me.MinimumSize = New System.Drawing.Size(575, 100)
        Me.Name = "FormClassShow"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "Loading Controls Dynamically - Grant Oberhauser"
        Me.MenuStrip1.ResumeLayout(False)
        Me.MenuStrip1.PerformLayout()
        Me.ContextMenuStrip1.ResumeLayout(False)
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Private Sub mnuReturn_Click(sender As Object, e As EventArgs) Handles mnuReturn.Click

        Me.Hide()

        _frmMain.Show()
        _frmMain.BringToFront()


    End Sub

    Private Sub ButtonClick(ByVal sender As Object,
                            ByVal e As System.EventArgs)
        Dim TheButton As Button

        TheButton = CType(sender, Button)

        MessageBox.Show("My Key: " + TheButton.Tag + Environment.NewLine + "My Caption: " + TheButton.Text)

    End Sub


    Private Sub mnuExit_Click(sender As Object, e As EventArgs) Handles mnuExit.Click

        Application.Exit()

    End Sub

    Private Sub FormClassShow_Resize(sender As Object, e As EventArgs) Handles MyBase.Resize
        ArrangeButtons()
    End Sub
End Class