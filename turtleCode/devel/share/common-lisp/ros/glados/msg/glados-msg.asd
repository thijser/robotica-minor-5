
(cl:in-package :asdf)

(defsystem "glados-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "music" :depends-on ("_package_music"))
    (:file "_package_music" :depends-on ("_package"))
  ))