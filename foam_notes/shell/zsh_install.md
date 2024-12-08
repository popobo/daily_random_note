# install zsh

```sh
sudo apt update

sudo apt install zsh git curl -y

chsh -s $(which zsh)
# ubuntu宿主机上需要log out再log in

sh -c "$(curl -fsSL https://install.ohmyz.sh/)"

# 修改~/.zshrc

# 配置主题

sudo wget -O $ZSH_CUSTOM/themes/haoomz.zsh-theme https://cdn.haoyep.com/gh/leegical/Blog_img/zsh/haoomz.zsh-theme

git clone --depth=1 https://github.com/romkatv/powerlevel10k.git ${ZSH_CUSTOM:-$HOME/.oh-my-zsh/custom}/themes/powerlevel10k

ZSH_THEME="powerlevel10k/powerlevel10k"

# 启动插件

git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions && git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting && git clone git@github.com:zdharma-continuum/history-search-multi-word.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/history-search-multi-word

plugins=(
    git
    zsh-autosuggestions
    zsh-syntax-highlighting
    z
    extract
    history-search-multi-word
)
```