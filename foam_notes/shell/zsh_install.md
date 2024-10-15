# install zsh

```sh
sudo apt update

sudo apt install zsh git curl -y

chsh -s /bin/zsh

sh -c "$(curl -fsSL https://install.ohmyz.sh/)"

# 修改~/.zshrc

# 配置主题

sudo wget -O $ZSH_CUSTOM/themes/haoomz.zsh-theme https://cdn.haoyep.com/gh/leegical/Blog_img/zsh/haoomz.zsh-theme

git clone --depth=1 https://github.com/romkatv/powerlevel10k.git ${ZSH_CUSTOM:-$HOME/.oh-my-zsh/custom}/themes/powerlevel10k

ZSH_THEME="powerlevel10k/powerlevel10k"

git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions

# 启动插件

plugins=(
    git
    zsh-autosuggestions
    zsh-syntax-highlighting
    z
    extract
)
```